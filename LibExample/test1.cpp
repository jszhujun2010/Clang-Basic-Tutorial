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

	// CompileInstance will hold the instance of Clang compiler for us,
	// managing the various objects needed to run the compiler.
	CompilerInstance Inst;

	// Diagnostics manage problems and issues in compile
	Inst.createDiagnostics(NULL, false);

	// TargetOptions *TO = new TargetOptions();
	// Note that shared pointer is used here.
	// Set target paltform options, initi target info with default triple for our platform.
	std::shared_ptr<clang::TargetOptions> TO = std::make_shared<clang::TargetOptions>();
	TO->Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *TI = TargetInfo::CreateTargetInfo(Inst.getDiagnostics(), TO);
	Inst.setTarget(TI);

	// FileManager supports for file system lookup, caching and directory search management.
	Inst.createFileManager();
	FileManager &FileMgr = Inst.getFileManager();

	// SourceManager handles loading and caching of source files into memory.
	Inst.createSourceManager(FileMgr);
	SourceManager &SourceMgr = Inst.getSourceManager();

	// Preprocessor runs within a single file.
	Inst.createPreprocessor(clang::TU_Complete);

	// ASTContext holds long-lived AST nodes (such as types and decls)
	Inst.createASTContext();

	// A Rewriter helps us manage the code rewriting task.
	Rewriter TheRewriter;

	TheRewriter.setSourceMgr(SourceMgr, Inst.getLangOpts());

	// Set the main file handled by the source manager to the input file.
	const FileEntry *FileIn = FileMgr.getFile(argv[1]);
	// New API is used.
	SourceMgr.setMainFileID(SourceMgr.createFileID(FileIn, clang::SourceLocation(), clang::SrcMgr::C_User));

	// Inform Diagnostics that processing of a source file is begining.
	Inst.getDiagnosticClient().BeginSourceFile(Inst.getLangOpts(), &Inst.getPreprocessor());

	// Create an AST consumer instance which is gong to be called by ParseAST.
	MyASTConsumer TheConsumer;

	// Parse the file to AST, registering our consumer as the ASTConsumer.
	ParseAST(Inst.getPreprocessor(), &TheConsumer, Inst.getASTContext());

	return 0;
}
