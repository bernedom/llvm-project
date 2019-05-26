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
  // if (!MatchedDecl->getName().startswith("operator\"\""))
  //   return;
  diag(MatchedDecl->getLocation(),
       "function %0 returns 'auto' instead of explict return type '%1'")
      << MatchedDecl << MatchedDecl->getReturnType().getAsString();
  diag(MatchedDecl->getLocation(), "replace with return type '%0'",
       DiagnosticIDs::Note)
      << FixItHint::CreateInsertion(MatchedDecl->getLocation(),
                                    MatchedDecl->getReturnType().getAsString())
      << MatchedDecl->getReturnType().getAsString();
}

} // namespace misc
} // namespace tidy
} // namespace clang
