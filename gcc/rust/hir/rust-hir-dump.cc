// Copyright (C) 2020-2023 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rust-hir-dump.h"

namespace Rust {
namespace HIR {

Dump::Dump (std::ostream &stream) : stream (stream) {}

void
Dump::put(std::string text, bool endline)
{
  // would need to split on lines here
  if (beg_of_line)
  {
    stream << indentation;
    beg_of_line = false;
  }

  // keep multiline string indented
  std::string::size_type pos = 0;
  std::string::size_type prev = 0;
  auto first = true;
  while ((pos = text.find('\n', prev)) != std::string::npos) {
    if (!first)
      stream << std::endl << indentation;

    first = false;
    stream << text.substr(prev, pos - prev);
    prev = pos + 1;
  }

  if (first)
    stream << text;

  if (endline)
  {
    stream << std::endl;
    beg_of_line = endline;
  }
}

static std::string delims[2][2] = {
{std::string("{"), std::string("}")},
{std::string("["), std::string("]")},
};

void
Dump::begin(std::string name, enum delim d)
{
  put("");
  put(name + " " + delims[d][0], true);
  indentation.increment();
}

void
Dump::end(std::string name, enum delim d)
{
  indentation.decrement ();
  stream << std::endl;
  put(delims[d][1] + " // " + name);
}

void
Dump::go (HIR::Crate &crate)
{
  begin("Crate");
  //stream << "Crate {" << std::endl;
  // inner attributes
  if (!crate.inner_attrs.empty ())
    {
      // indentation.increment ();
      // stream << indentation;
      put("inner_attrs: [", false);
      //stream << "inner_attrs: [";
      for (auto &attr : crate.inner_attrs)
	put(attr.as_string(), false);
	//stream << attr.as_string ();
      put("]", true);
      //stream << "]," << std::endl;
      //indentation.decrement ();
    }

  // indentation.increment ();
  // stream << indentation;

  begin("items", SQUARE);
  //stream << "items: [";

  //stream << indentation;
  for (const auto &item : crate.items)
    {
//      stream << std::endl;
      item->accept_vis (*this);
    }

  end("items", SQUARE);
  // stream << std::endl;
  // stream << indentation;

  // stream << "]," << std::endl;
  // indentation.decrement ();
  //

  // indentation.increment ();
  // stream << indentation;
  // stream << "node_mappings: ";
  put("node_mapping: ", false);
  //stream << crate.get_mappings ().as_string ();
  put(crate.get_mappings ().as_string (), true);

  end("Crate");

  // indentation.decrement ();

  // stream << "\n}" << std::endl;
}

void
Dump::visit (AST::Attribute &attribute)
{
  std::string path_str = attribute.get_path ().as_string ();
  put(path_str, false);
  //stream << path_str;
  if (attribute.has_attr_input ())
    put(attribute.get_attr_input ().as_string (), false);
    //stream << attribute.get_attr_input ().as_string ();
}

void
Dump::visit (Lifetime &lifetime)
{
  if (lifetime.is_error ())
    {
      put("ERROR-MARK-STRING error lifetime ", false);
      // stream << "error lifetime";
      return;
    }

  switch (lifetime.get_lifetime_type ())
    {
    case AST::Lifetime::LifetimeType::NAMED:
      put("'" + lifetime.get_name(), false);
      //stream << "'" << lifetime.get_name ();
      break;
    case AST::Lifetime::LifetimeType::STATIC:
      put("'static", false);
      //stream << "'static";
      break;
    case AST::Lifetime::LifetimeType::WILDCARD:
      put("'_", false);
      //stream << "'_";
      break;
    default:
      //stream << "ERROR-MARK-STRING: lifetime type failure";
      put("ERROR-MARK-STRING: lifetime type failure", false);
      break;
    }
}
void
Dump::visit (LifetimeParam &)
{
  begin("LifetimeParam (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("LifetimeParam");
}

void
Dump::visit (PathInExpression &)
{
  begin("PathInExpression (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("PathInExpression");
}

void
Dump::visit (TypePathSegment &)
{
  begin("TypePathSegment (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypePathSegment");
}
void
Dump::visit (TypePathSegmentGeneric &)
{
  begin("TypePathSegmentGeneric (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypePathSegmentGeneric");
}
void
Dump::visit (TypePathSegmentFunction &)
{
  begin("TypePathSegmentFunction (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypePathSegmentFunction");
}
void
Dump::visit (TypePath &)
{
  begin("TypePath (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypePath");
}
void
Dump::visit (QualifiedPathInExpression &)
{
  begin("QualifiedPathInExpression (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("QualifiedPathInExpression");
}
void
Dump::visit (QualifiedPathInType &)
{
  begin("QualifiedPathInType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("QualifiedPathInType");
}

void
Dump::visit (LiteralExpr &literal_expr)
{
  put (literal_expr.get_literal ().as_string () + " " + literal_expr.get_mappings ().as_string (), false);
}

void
Dump::visit (BorrowExpr &)
{
  begin("BorrowExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("BorrowExpr");
}

void
Dump::visit (DereferenceExpr &)
{
  begin("DereferenceExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("DereferenceExpr");
}
void
Dump::visit (ErrorPropagationExpr &)
{
  begin("ErrorPropagationExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ErrorPropagationExpr");
}
void
Dump::visit (NegationExpr &)
{
  begin("NegationExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("NegationExpr");
}
void
Dump::visit (ArithmeticOrLogicalExpr &aole)
{
  std::string operator_str;
  operator_str.reserve (1);

  // which operator
  switch (aole.get_expr_type ())
    {
    case ArithmeticOrLogicalOperator::ADD:
      operator_str = "+";
      break;
    case ArithmeticOrLogicalOperator::SUBTRACT:
      operator_str = "-";
      break;
    case ArithmeticOrLogicalOperator::MULTIPLY:
      operator_str = "*";
      break;
    case ArithmeticOrLogicalOperator::DIVIDE:
      operator_str = "/";
      break;
    case ArithmeticOrLogicalOperator::MODULUS:
      operator_str = "%";
      break;
    case ArithmeticOrLogicalOperator::BITWISE_AND:
      operator_str = "&";
      break;
    case ArithmeticOrLogicalOperator::BITWISE_OR:
      operator_str = "|";
      break;
    case ArithmeticOrLogicalOperator::BITWISE_XOR:
      operator_str = "^";
      break;
    case ArithmeticOrLogicalOperator::LEFT_SHIFT:
      operator_str = "<<";
      break;
    case ArithmeticOrLogicalOperator::RIGHT_SHIFT:
      operator_str = ">>";
      break;
    default:
      gcc_unreachable ();
      break;
    }

  aole.visit_lhs (*this);
  put("", true);
  //stream << "\n";
  put(operator_str, true);
  // stream << indentation;
  // stream << operator_str << "\n";
  // stream << indentation;
  aole.visit_rhs (*this);
}
void
Dump::visit (ComparisonExpr &)
{
  begin("ComparisonExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ComparisonExpr");
}
void
Dump::visit (LazyBooleanExpr &)
{
  begin("LazyBooleanExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("LazyBooleanExpr");
}
void
Dump::visit (TypeCastExpr &)
{
  begin("TypeCastExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypeCastExpr");
}
void
Dump::visit (AssignmentExpr &)
{
  begin("AssignmentExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("AssignmentExpr");
}
void
Dump::visit (CompoundAssignmentExpr &)
{
  begin("CompoundAssignmentExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("CompoundAssignmentExpr");
}
void
Dump::visit (GroupedExpr &)
{
  begin("GroupedExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("GroupedExpr");
}

void
Dump::visit (ArrayElemsValues &)
{
  begin("ArrayElemsValues (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ArrayElemsValues");
}
void
Dump::visit (ArrayElemsCopied &)
{
  begin("ArrayElemsCopied (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ArrayElemsCopied");
}
void
Dump::visit (ArrayExpr &expr)
{
  begin("ArrayExpr");

  auto attrs = expr.get_inner_attrs ();
  do_inner_attrs(attrs);

  // FIXME do outer_attrs
  put (expr.as_string (), true);

  end("ArrayExpr");
}
void
Dump::visit (ArrayIndexExpr &)
{
  begin("ArrayIndexExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ArrayIndexExpr");
}
void
Dump::visit (TupleExpr &)
{
  begin("TupleExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleExpr");
}
void
Dump::visit (TupleIndexExpr &)
{
  begin("TupleIndexExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleIndexExpr");
}
void
Dump::visit (StructExprStruct &)
{
  begin("StructExprStruct (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructExprStruct");
}

void
Dump::visit (StructExprFieldIdentifier &)
{
  begin("StructExprFieldIdentifier (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructExprFieldIdentifier");
}
void
Dump::visit (StructExprFieldIdentifierValue &)
{
  begin("StructExprFieldIdentifierValue (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructExprFieldIdentifierValue");
}

void
Dump::visit (StructExprFieldIndexValue &)
{
  begin("StructExprFieldIndexValue (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructExprFieldIndexValue");
}
void
Dump::visit (StructExprStructFields &)
{
  begin("StructExprStructFields (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructExprStructFields");
}
void
Dump::visit (StructExprStructBase &)
{
  begin("StructExprStructBase (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructExprStructBase");
}

void
Dump::visit (CallExpr &)
{
  begin("CallExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("CallExpr");
}
void
Dump::visit (MethodCallExpr &)
{
  begin("MethodCallExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("MethodCallExpr");
}
void
Dump::visit (FieldAccessExpr &)
{
  begin("FieldAccessExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("FieldAccessExpr");
}
void
Dump::visit (ClosureExpr &)
{
  begin("ClosureExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ClosureExpr");
}

void
Dump::do_inner_attrs(std::vector<AST::Attribute> &attrs)
{
  if (attrs.empty ())
    return;

  begin("inner_attrs", SQUARE);
  // stream << indentation << "inner_attrs: [";
  // indentation.increment ();
  for (auto &attr : attrs)
  {
    // stream << "\n";
    // stream << indentation;
    visit (attr);
  }
  end("inner_attrs", SQUARE);
  // indentation.decrement ();
  // stream << "\n" << indentation << "]\n";
}


void
Dump::visit (BlockExpr &block_expr)
{
  //stream << "BlockExpr: [\n";
  begin("BlockExpr", SQUARE);

  //indentation.increment ();
  // TODO: inner attributes
  auto ia = block_expr.get_inner_attrs();
  do_inner_attrs(ia);
  // stream << indentation << "inner_attrs: [";
  // indentation.increment ();
  // for (auto &attr : block_expr.inner_attrs)
  // 	{
  // 	  stream << "\n";
  // 	  stream << indentation;
  // 	  visit (attr);
  // 	}
  // indentation.decrement ();
  // stream << "\n" << indentation << "]\n";


  // statements
  // impl null pointer check

  if (block_expr.has_statements ())
    {
      auto &stmts = block_expr.get_statements ();
      for (auto &stmt : stmts)
	{
	  begin("Stmt");
	  //stream << indentation << "Stmt: {\n";
	  stmt->accept_vis (*this);
	  end("Stmt");
	  // stream << "\n";
	  // stream << indentation << "}\n";
	}
    }

  // final expression
  if (block_expr.has_expr ())
    {
      put ("final expression: " + block_expr.expr->as_string (), true);
      //stream << indentation << "final expression:";
      //stream << "\n" << indentation << block_expr.expr->as_string ();
    }
  end ("BlockExpr");
  // indentation.decrement ();
  //stream << "\n" << indentation << "]";
}

void
Dump::visit (ContinueExpr &)
{
  begin("ContinueExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ContinueExpr");
}
void
Dump::visit (BreakExpr &)
{
  begin("BreakExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("BreakExpr");
}
void
Dump::visit (RangeFromToExpr &)
{
  begin("RangeFromToExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangeFromToExpr");
}
void
Dump::visit (RangeFromExpr &)
{
  begin("RangeFromExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangeFromExpr");
}
void
Dump::visit (RangeToExpr &)
{
  begin("RangeToExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangeToExpr");
}
void
Dump::visit (RangeFullExpr &)
{
  begin("RangeFullExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangeFullExpr");
}
void
Dump::visit (RangeFromToInclExpr &)
{
  begin("RangeFromToInclExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangeFromToInclExpr");
}
void
Dump::visit (RangeToInclExpr &)
{
  begin("RangeToInclExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangeToInclExpr");
}
void
Dump::visit (ReturnExpr &)
{
  begin("ReturnExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ReturnExpr");
}
void
Dump::visit (UnsafeBlockExpr &)
{
  begin("UnsafeBlockExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("UnsafeBlockExpr");
}
void
Dump::visit (LoopExpr &)
{
  begin("LoopExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("LoopExpr");
}
void
Dump::visit (WhileLoopExpr &)
{
  begin("WhileLoopExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("WhileLoopExpr");
}
void
Dump::visit (WhileLetLoopExpr &)
{
  begin("WhileLetLoopExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("WhileLetLoopExpr");
}
void
Dump::visit (ForLoopExpr &)
{
  begin("ForLoopExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ForLoopExpr");
}
void
Dump::visit (IfExpr &if_expr)
{
  begin("IfExpr");
  // stream << indentation << "IfExpr: [\n";
  // indentation.increment ();

  begin("condition");
  //stream << indentation << "condition: [\n";
  if_expr.vis_if_condition (*this);
  end("condition");
  //stream << indentation << "]\n";

  begin("if_block");
  //stream << indentation << "if_block: [\n";
  if_expr.vis_if_block (*this);
//  stream << indentation << "]\n";
  end("if_block");

  end("IfExpr");
  //indentation.decrement ();
  //stream << "\n" << indentation << "]";
}

void
Dump::visit (IfExprConseqElse &)
{
  begin("IfExprConseqElse (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("IfExprConseqElse");
}
void
Dump::visit (IfLetExpr &)
{
  begin("IfLetExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("IfLetExpr");
}
void
Dump::visit (IfLetExprConseqElse &)
{
  begin("IfLetExprConseqElse (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("IfLetExprConseqElse");
}

void
Dump::visit (MatchExpr &)
{
  begin("MatchExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("MatchExpr");
}
void
Dump::visit (AwaitExpr &)
{
  begin("AwaitExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("AwaitExpr");
}
void
Dump::visit (AsyncBlockExpr &)
{
  begin("AsyncBlockExpr (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("AsyncBlockExpr");
}

void
Dump::visit (TypeParam &)
{
  begin("TypeParam (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypeParam");
}

void
Dump::visit (ConstGenericParam &)
{
  begin("ConstGenericParam (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ConstGenericParam");
}

void
Dump::visit (LifetimeWhereClauseItem &)
{
  begin("LifetimeWhereClauseItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("LifetimeWhereClauseItem");
}
void
Dump::visit (TypeBoundWhereClauseItem &)
{
  begin("TypeBoundWhereClauseItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypeBoundWhereClauseItem");
}
void
Dump::visit (Module &)
{
  begin("Module (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("Module");
}
void
Dump::visit (ExternCrate &)
{
  begin("ExternCrate (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ExternCrate");
}

void
Dump::visit (UseTreeGlob &)
{
  begin("UseTreeGlob (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("UseTreeGlob");
}
void
Dump::visit (UseTreeList &)
{
  begin("UseTreeList (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("UseTreeList");
}
void
Dump::visit (UseTreeRebind &)
{
  begin("UseTreeRebind (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("UseTreeRebind");
}
void
Dump::visit (UseDeclaration &)
{
  begin("UseDeclaration (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("UseDeclaration");
}
void
Dump::visit (Function &func)
  {
  begin("Function", CURLY);
  // indentation.increment ();
  // stream << indentation << "Function {" << std::endl;
  // indentation.increment ();

  // function name
  put("func_name: " + func.get_function_name () + ",", true);
  //stream << indentation << "func_name: ";
  // auto func_name = func.get_function_name ();
  // stream << func_name;
  // stream << ",\n";

  // return type
  put("return_type: ", false);
  //stream << indentation << "return_type: ";
  if (func.has_return_type ())
    {
      auto &ret_type = func.get_return_type ();
      put (ret_type->as_string () + ",", true);
      // stream << ret_type->as_string ();
      // stream << ",\n";
    }
  else
    {
      put ("void,", true);
      //stream << "void,\n";
    }

  // function params
  if (func.has_function_params ())
    {
      begin("params");
      //stream << indentation << "params: [\n";
      //indentation.increment ();
      auto &func_params = func.get_function_params ();
      for (const auto &item : func_params)
	{
	  put(item.as_string()+ ",", false);
	  //stream << indentation << item.as_string () << ",\n";
	}
      end("params");

      // parameter node mappings
      begin("node_mappings");
      //stream << indentation << "node_mappings: [\n";
      for (const auto &item : func_params)
	{
	  auto nmap = item.get_mappings ();
	  //indentation.increment ();
	  // stream << indentation;
	  auto pname = item.param_name->as_string ();
	  put(pname + ":" + nmap.as_string () + ",", true);
	  // stream << pname << ": ";
	  // stream << nmap.as_string () << ",\n";
	  // indentation.decrement ();
	}
      end("params");
      end("node_mappings");
      // stream << indentation << "],";
      // indentation.decrement ();
      // stream << "\n";
      // stream << indentation << "],";
      // stream << "\n";
    }

  // function body
  //stream << indentation;
  auto &func_body = func.get_definition ();
  func_body->accept_vis (*this);

  // func node mappings
  //stream << "\n";
  put("node_mappings: " + func.get_impl_mappings ().as_string (), true);
  // stream << indentation << "node_mappings: ";
  // stream << func.get_impl_mappings ().as_string ();
  // indentation.decrement ();
  // stream << "\n";
  // stream << indentation << "}" << std::endl;
  // // TODO: get function definition and visit block

  // // stream << std::endl;
  // indentation.decrement ();
  end("Function", CURLY);
}

void
Dump::visit (TypeAlias &t)
{
  begin("TypeAlias (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TypeAlias");
}

void
Dump::visit (StructStruct &)
{
  begin("StructStruct (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructStruct");
}

void
Dump::visit (TupleStruct &)
{
  begin("TupleStruct (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleStruct");
}
void
Dump::visit (EnumItem &)
{
  begin("EnumItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("EnumItem");
}
void
Dump::visit (EnumItemTuple &)
{
  begin("EnumItemTuple (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("EnumItemTuple");
}
void
Dump::visit (EnumItemStruct &)
{
  begin("EnumItemStruct (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("EnumItemStruct");
}
void
Dump::visit (EnumItemDiscriminant &)
{
  begin("EnumItemDiscriminant (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("EnumItemDiscriminant");
}
void
Dump::visit (Enum &)
{
  begin("Enum (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("Enum");
}
void
Dump::visit (Union &)
{
  begin("Union (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("Union");
}
void
Dump::visit (ConstantItem &)
{
  begin("ConstantItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ConstantItem");
}
void
Dump::visit (StaticItem &)
{
  begin("StaticItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StaticItem");
}
void
Dump::visit (TraitItemFunc &)
{
  begin("TraitItemFunc (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TraitItemFunc");
}
void
Dump::visit (TraitItemConst &)
{
  begin("TraitItemConst (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TraitItemConst");
}
void
Dump::visit (TraitItemType &)
{
  begin("TraitItemType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TraitItemType");
}
void
Dump::visit (Trait &)
{
  begin("Trait (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("Trait");
}
void
Dump::visit (ImplBlock &)
{
  begin("ImplBlock (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ImplBlock");
}

void
Dump::visit (ExternalStaticItem &)
{
  begin("ExternalStaticItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ExternalStaticItem");
}
void
Dump::visit (ExternalFunctionItem &)
{
  begin("ExternalFunctionItem (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ExternalFunctionItem");
}
void
Dump::visit (ExternBlock &)
{
  begin("ExternBlock (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ExternBlock");
}

void
Dump::visit (LiteralPattern &)
{
  begin("LiteralPattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("LiteralPattern");
}
void
Dump::visit (IdentifierPattern &ident)
{
  auto ident_name = ident.get_identifier ();
  put(ident_name, false);
  //stream << ident_name;
}
void
Dump::visit (WildcardPattern &)
{
  begin("WildcardPattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("WildcardPattern");
}

void
Dump::visit (RangePatternBoundLiteral &)
{
  begin("RangePatternBoundLiteral (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangePatternBoundLiteral");
}
void
Dump::visit (RangePatternBoundPath &)
{
  begin("RangePatternBoundPath (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangePatternBoundPath");
}
void
Dump::visit (RangePatternBoundQualPath &)
{
  begin("RangePatternBoundQualPath (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangePatternBoundQualPath");
}
void
Dump::visit (RangePattern &)
{
  begin("RangePattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RangePattern");
}
void
Dump::visit (ReferencePattern &)
{
  begin("ReferencePattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ReferencePattern");
}

void
Dump::visit (StructPatternFieldTuplePat &)
{
  begin("StructPatternFieldTuplePat (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructPatternFieldTuplePat");
}
void
Dump::visit (StructPatternFieldIdentPat &)
{
  begin("StructPatternFieldIdentPat (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructPatternFieldIdentPat");
}
void
Dump::visit (StructPatternFieldIdent &)
{
  begin("StructPatternFieldIdent (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructPatternFieldIdent");
}
void
Dump::visit (StructPattern &)
{
  begin("StructPattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("StructPattern");
}

void
Dump::visit (TupleStructItemsNoRange &)
{
  begin("TupleStructItemsNoRange (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleStructItemsNoRange");
}
void
Dump::visit (TupleStructItemsRange &)
{
  begin("TupleStructItemsRange (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleStructItemsRange");
}
void
Dump::visit (TupleStructPattern &)
{
  begin("TupleStructPattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleStructPattern");
}

void
Dump::visit (TuplePatternItemsMultiple &)
{
  begin("TuplePatternItemsMultiple (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TuplePatternItemsMultiple");
}
void
Dump::visit (TuplePatternItemsRanged &)
{
  begin("TuplePatternItemsRanged (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TuplePatternItemsRanged");
}
void
Dump::visit (TuplePattern &)
{
  begin("TuplePattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TuplePattern");
}
void
Dump::visit (SlicePattern &)
{
  begin("SlicePattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("SlicePattern");
}
void
Dump::visit (AltPattern &)
{
  begin("AltPattern (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("AltPattern");
}

void
Dump::visit (EmptyStmt &)
{
  begin("EmptyStmt (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("EmptyStmt");
}
void
Dump::visit (LetStmt &let_stmt)
{
  begin("LetStmt", CURLY);
  // indentation.increment ();
  // TODO: outer attributes
  // stream << indentation << "LetStmt: {\n";
  // indentation.increment ();
  // stream << indentation;

  auto var_pattern = let_stmt.get_pattern ();
  put(var_pattern->as_string (), false);
  //stream << var_pattern->as_string ();
  // return type
  if (let_stmt.has_type ())
    {
      auto ret_type = let_stmt.get_type ();
      put(std::string(": ") + ret_type->as_string (), false);
      //stream << ": " << ret_type->as_string ();
    }

  // init expr
  if (let_stmt.has_init_expr ())
    {
      begin("Expr", CURLY);
      // stream << " = Expr: {\n ";
      // indentation.increment ();
      // stream << indentation;
      auto expr = let_stmt.get_init_expr ();
      expr->accept_vis (*this);
      end("Expr", CURLY);
      // stream << "\n";
      // stream << indentation << "}\n";
      // indentation.decrement ();
    }
  // indentation.decrement ();
  // stream << indentation << "}\n";

  // indentation.decrement ();
  end("LetStmt", CURLY);
}

void
Dump::visit (ExprStmt &expr_stmt)
{
  auto expr = expr_stmt.get_expr ();
  expr->accept_vis (*this);
}

void
Dump::visit (TraitBound &)
{
  begin("TraitBound (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TraitBound");
}
void
Dump::visit (ImplTraitType &)
{
  begin("ImplTraitType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ImplTraitType");
}
void
Dump::visit (TraitObjectType &)
{
  begin("TraitObjectType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TraitObjectType");
}
void
Dump::visit (ParenthesisedType &)
{
  begin("ParenthesisedType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ParenthesisedType");
}
void
Dump::visit (ImplTraitTypeOneBound &)
{
  begin("ImplTraitTypeOneBound (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ImplTraitTypeOneBound");
}
void
Dump::visit (TupleType &)
{
  begin("TupleType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("TupleType");
}
void
Dump::visit (NeverType &)
{
  begin("NeverType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("NeverType");
}
void
Dump::visit (RawPointerType &)
{
  begin("RawPointerType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("RawPointerType");
}
void
Dump::visit (ReferenceType &)
{
  begin("ReferenceType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("ReferenceType");
}
void
Dump::visit (ArrayType & at)
{
  put (std::string("[") + at.get_element_type()->as_string() + ";", false);
  at.get_size_expr()->accept_vis(*this);
  put("]", false);
}

void
Dump::visit (SliceType & slicetype)
{
  put (std::string("&[") + slicetype.get_element_type ()->as_string() + "]", false);
}

void
Dump::visit (InferredType &)
{
  begin("InferredType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("InferredType");
}
void
Dump::visit (BareFunctionType &)
{
  begin("BareFunctionType (INCOMPLETE -- CONTENT NOT DISPLAYED)");
  end("BareFunctionType");
}
} // namespace HIR
} // namespace Rust
