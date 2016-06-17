#include "types/DecimalLit.hpp"
#include "types/DecimalType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(DecimalTypeTest, PrintValueTest) {
  const Type &decimal_type = TypeFactory::GetType(kDecimal);

  // Try integer version.
  DecimalLit integer_decimal = DecimalLit::fromInt(36509);
  TypedValue value_for_integer_decimal(integer_decimal);
  EXPECT_EQ(std::string("36509.00"),
            decimal_type.printValueToString(value_for_integer_decimal));

  // Try double version.
  DecimalLit double_decimal = DecimalLit::fromDouble(36509.65);
  TypedValue value_for_double_decimal(double_decimal);
  EXPECT_EQ(std::string("36509.65"),
            decimal_type.printValueToString(value_for_double_decimal));

  // Try truncation of double version.
  DecimalLit double_decimal_truncated = DecimalLit::fromDouble(36509.6526762);
  TypedValue value_for_double_decimal_truncated(double_decimal_truncated);
  EXPECT_EQ(std::string("36509.65"),
            decimal_type.printValueToString(
                value_for_double_decimal_truncated));

  // Test that number is truncated, not rounded.
  double_decimal_truncated = DecimalLit::fromDouble(36509.6599999);
  TypedValue value_for_double_decimal_truncated_other(double_decimal_truncated);
  EXPECT_EQ(std::string("36509.65"),
            decimal_type.printValueToString(
                value_for_double_decimal_truncated));
}

TEST(DecimalTypeTest, DecimalLitOperationsTest) {
  const Type &decimal_type = TypeFactory::GetType(kDecimal);

  DecimalLit decimal_a = DecimalLit::fromDouble(560.35);
  DecimalLit decimal_b = DecimalLit::fromDouble(439.65);

  EXPECT_EQ(std::string("1000.00"),
            decimal_type.printValueToString(TypedValue(decimal_a + decimal_b)));

  EXPECT_EQ(std::string("120.70"),
            decimal_type.printValueToString(TypedValue(decimal_a - decimal_b)));

  EXPECT_EQ(std::string("246357.87"),
            decimal_type.printValueToString(TypedValue(decimal_a * decimal_b)));

  EXPECT_EQ(std::string("1.27"),
            decimal_type.printValueToString(TypedValue(decimal_a / decimal_b)));

  EXPECT_EQ(std::string("120.70"),
            decimal_type.printValueToString(TypedValue(decimal_a % decimal_b)));
}


}
