#include "DesignExtractor.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include "Visitor/SetLineNumberVisitor.h"
#include "Visitor/ExtractNextVisitor.h"
#include "Visitor/ExtractEntitiesVisitor.h"
#include "Visitor/ExtractCallsVisitor.h"
#include "Visitor/ExtractFollowsVisitor.h"
#include "Visitor/ExtractParentVisitor.h"
#include "Visitor/ExtractModifiesVisitor.h"
#include "Visitor/ExtractUsesVisitor.h"
#include "Visitor/ExtractPatternVisitor.h"

DesignExtractor::~DesignExtractor() {}

void DesignExtractor::extractDesignAbstractions(const std::shared_ptr<ProgramNode> astRoot) {
    SetLineNumberVisitor* setLineNumberVisitor = new SetLineNumberVisitor(pkbFacade);
    ExtractNextVisitor* extractNextVisitor = new ExtractNextVisitor(pkbFacade);
    ExtractEntitiesVisitor* extractEntitiesVisitor = new ExtractEntitiesVisitor(pkbFacade);
    ExtractCallsVisitor* extractCallsVisitor = new ExtractCallsVisitor(pkbFacade);
    ExtractFollowsVisitor* extractFollowsVisitor = new ExtractFollowsVisitor(pkbFacade);
    ExtractParentVisitor* extractParentVisitor = new ExtractParentVisitor(pkbFacade);
    ExtractModifiesVisitor* extractModifiesVisitor = new ExtractModifiesVisitor(pkbFacade);
    ExtractUsesVisitor* extractUsesVisitor = new ExtractUsesVisitor(pkbFacade);
    ExtractPatternVisitor* extractPatternVisitor = new ExtractPatternVisitor(pkbFacade);

    astRoot->accept(setLineNumberVisitor);
    astRoot->accept(extractNextVisitor);
    astRoot->accept(extractEntitiesVisitor);
    astRoot->accept(extractCallsVisitor);
    astRoot->accept(extractFollowsVisitor);
    astRoot->accept(extractParentVisitor);
    astRoot->accept(extractModifiesVisitor);
    astRoot->accept(extractUsesVisitor);
    astRoot->accept(extractPatternVisitor);

    delete setLineNumberVisitor;
    delete extractNextVisitor;
    delete extractEntitiesVisitor;
    delete extractCallsVisitor;
    delete extractFollowsVisitor;
    delete extractParentVisitor;
    delete extractModifiesVisitor;
    delete extractUsesVisitor;
    delete extractPatternVisitor;
}
