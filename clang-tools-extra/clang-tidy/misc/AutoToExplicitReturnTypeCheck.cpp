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
  Finder->addMatcher(
      functionDecl(isConstexpr(), unless(isImplicit()), returns(autoType()))
          .bind("x"),
      this);
  // Finder->addMatcher(userDefinedLiteral().bind("x"), this);
}

void AutoToExplicitReturnTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("x");

  // Set printing policy to remove 'struct'
  PrintingPolicy pol(getLangOpts());
  pol.SuppressSpecifiers = 0;

  diag(MatchedDecl->getReturnTypeSourceRange().getBegin(),
       "function %0 returns 'auto' instead of explict return type '%1'")
      << MatchedDecl << MatchedDecl->getReturnType().getAsString(pol);
  diag(MatchedDecl->getReturnTypeSourceRange().getBegin(),
       "replace with return type '%0'", DiagnosticIDs::Note)
      << FixItHint::CreateReplacement(
             MatchedDecl->getReturnTypeSourceRange(),
             MatchedDecl->getReturnType().getAsString(pol))
      << MatchedDecl->getReturnType().getAsString(pol);
}

} // namespace misc
} // namespace tidy
} // namespace clang
