//===--- AutoToExplicitReturnTypeCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AutoToExplicitReturnTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void AutoToExplicitReturnTypeCheck::registerMatchers(MatchFinder *Finder) {

  // only parse c++
  if (!getLangOpts().CPlusPlus)
    return;

  Finder->addMatcher(
      functionDecl(unless(isImplicit()), returns(autoType()),
                   hasDescendant(returnStmt().bind("return_stmt")))
          .bind("func_decl"),
      this);
}

void AutoToExplicitReturnTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("func_decl");
  const auto *rstat = Result.Nodes.getNodeAs<ReturnStmt>("return_stmt");

  // Set printing policy to remove 'struct'
  PrintingPolicy pol(getLangOpts());
  pol.SuppressSpecifiers = 0;

  auto return_type = rstat->getRetValue()->getType();
  if (!return_type.getTypePtr()->isDependentType()) {
    diag(MatchedDecl->getReturnTypeSourceRange().getBegin(),
         "function %0 returns 'auto' instead of explict return type '%1'")
        << MatchedDecl << return_type.getAsString(pol);
    diag(MatchedDecl->getReturnTypeSourceRange().getBegin(),
         "replace with return type '%0'", DiagnosticIDs::Note)
        << FixItHint::CreateReplacement(MatchedDecl->getReturnTypeSourceRange(),
                                        return_type.getAsString(pol))
        << return_type.getAsString(pol);
  }
}

} // namespace misc
} // namespace tidy
} // namespace clang
