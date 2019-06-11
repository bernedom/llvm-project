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

  auto declMatcher =
      functionDecl(isConstexpr(), unless(isImplicit()), returns(autoType()));
  Finder->addMatcher(
      cxxFunctionalCastExpr(hasAncestor(declMatcher.bind("func_decl")))
          .bind("cast_expr"),
      this);

  Finder->addMatcher(
      functionDecl(isConstexpr(), unless(isImplicit()), returns(autoType()),
                   unless(hasDescendant(cxxFunctionalCastExpr())))
          .bind("func_decl"),
      this);
}

void AutoToExplicitReturnTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("func_decl");
  const auto *cast_expr = Result.Nodes.getNodeAs<ExplicitCastExpr>("cast_expr");

  // Set printing policy to remove 'struct'
  PrintingPolicy pol(getLangOpts());
  pol.SuppressSpecifiers = 0;

  auto return_type = MatchedDecl->getReturnType();
  if (cast_expr) {
    return_type = cast_expr->getType();
  }

  diag(MatchedDecl->getReturnTypeSourceRange().getBegin(),
       "function %0 returns 'auto' instead of explict return type '%1'")
      << MatchedDecl << return_type.getAsString(pol);
  diag(MatchedDecl->getReturnTypeSourceRange().getBegin(),
       "replace with return type '%0'", DiagnosticIDs::Note)
      << FixItHint::CreateReplacement(MatchedDecl->getReturnTypeSourceRange(),
                                      return_type.getAsString(pol))
      << return_type.getAsString(pol);
}

} // namespace misc
} // namespace tidy
} // namespace clang
