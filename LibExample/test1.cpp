#include <clang/AST/ASTConsumer.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Rewrite/Frontend/Rewriters.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Lex/Lexer.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Parse/ParseAST.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <assert.h>

using namespace std;
using namespace clang;

class MyASTVisitor: public RecursiveASTVisitor<MyASTVisitor> {
public:
	virtual bool VisitStmt(Stmt *s) {
		std::cout << s->getStmtClassName() << "\n";
		return true;
	}

	virtual bool VisitFunctionDecl(FunctionDecl *f) {
		//std::cout << f->getName() << "\n";
		cout << f->getNameInfo().getName().getAsString() << "\n";
		return true;
	}
};

class MyASTConsumer: public ASTConsumer {
public:
	MyASTConsumer(): Visitor() {}

	virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
		for(DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
			Visitor.TraverseDecl(*b);
		}
		return true;
	}

private:
	MyASTVisitor Visitor;
};


int main(int argc, char *argv[]) {
	if (argc != 2) {
		llvm::errs() << "Usage: PrintFunctions <filename>\n";
		return 1;
	}

	CompilerInstance Inst;
	Inst.createDiagnostics(NULL, false);

	//TargetOptions *TO = new TargetOptions();
	//Note that shared pointer is used here.
	std::shared_ptr<clang::TargetOptions> TO = std::make_shared<clang::TargetOptions>();
	TO->Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *TI = TargetInfo::CreateTargetInfo(Inst.getDiagnostics(), TO);
	Inst.setTarget(TI);

	Inst.createFileManager();
	FileManager &FileMgr = Inst.getFileManager();

	Inst.createSourceManager(FileMgr);
	SourceManager &SourceMgr = Inst.getSourceManager();

	Inst.createPreprocessor(clang::TU_Complete);

	Inst.createASTContext();

	Rewriter TheRewriter;

	TheRewriter.setSourceMgr(SourceMgr, Inst.getLangOpts());

	const FileEntry *FileIn = FileMgr.getFile(argv[1]);
	//Seems new API is used.
	SourceMgr.setMainFileID(SourceMgr.createFileID(FileIn, clang::SourceLocation(), clang::SrcMgr::C_User));

	Inst.getDiagnosticClient().BeginSourceFile(Inst.getLangOpts(), &Inst.getPreprocessor());

	MyASTConsumer TheConsumer;

	ParseAST(Inst.getPreprocessor(), &TheConsumer, Inst.getASTContext());

	return 0;
}
