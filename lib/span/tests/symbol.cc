#include <gtest/gtest.h>
#include <span/symbol.hh>

namespace crust {

class InternerTest : public ::testing::Test {
protected:
  Interner _i = {};
};

TEST_F(InternerTest, interner_test) {
  ASSERT_EQ(_i.intern("cat"), Symbol(0));
  ASSERT_EQ(_i.intern("cat"), Symbol(0));
}

TEST_F(InternerTest, interner_test2) {
  ASSERT_EQ(_i.intern("dog"), Symbol(1));
  ASSERT_EQ(_i.intern("dog"), Symbol(1));
}

TEST_F(InternerTest, interner_test3) {
  ASSERT_EQ(_i.intern("cat"), Symbol(0));
  ASSERT_EQ(_i.intern("dog"), Symbol(1));
}

TEST(KeywordInterner, interner_test4) {
  auto i = Interner();
  i.clear();
  i.fresh();
  ASSERT_EQ(Ident::from_str("as").name, kw::As);
  ASSERT_EQ(Ident::from_str("break").name, kw::Break);
}

} // namespace crust
