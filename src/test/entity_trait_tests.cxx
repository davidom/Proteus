#include <geometry/entity_trait.hxx>
#include "gtest/gtest.h"

class EntityTraitTestingF : public ::testing::Test {
  protected:
    virtual void SetUp() {
	  size0 = 0;
	  index = 1;
	  value = 1;
	  index1 = 1; index2 = 2; index3 = 3;
	  value1 = 1; value2 = 2; value3 = 3;
	  large_index = 100;
	  all_value = 4.0;
	}

    Proteus::entity_trait<int> et_i;
    Proteus::entity_trait<double> et_d;
	int size0, index, value, large_index, index1, index2, index3;
    int value1, value2, value3;
	double all_value;
};

TEST_F(EntityTraitTestingF, CreationEmpty)
{
  EXPECT_EQ(et_i.size(),size0);
}

TEST_F(EntityTraitTestingF, IsEqualWhenEmpty)
{
  EXPECT_EQ(et_i.is_equal(index,value), false);
}

TEST_F(EntityTraitTestingF, SetTrait)
{
  et_i.set_trait(index,value);
  EXPECT_EQ(et_i.is_equal(index,value), true);
  EXPECT_NO_THROW(et_i.is_equal(large_index,value));
  EXPECT_EQ(et_i.is_equal(index,value+1), false);
}

TEST_F(EntityTraitTestingF, UnsetTrait)
{
  et_i.set_trait(index,value);
  EXPECT_EQ(et_i.is_equal(index,value), true);
  et_i.unset_trait(index);
  EXPECT_EQ(et_i.is_equal(index,value), false);
  EXPECT_NO_THROW(et_i.unset_trait(large_index));
}


TEST_F(EntityTraitTestingF, UnsetEach)
{
  et_i.set_trait(index1,value1);
  et_i.set_trait(index2,value2);
  et_i.set_trait(index3,value3);
  EXPECT_EQ(et_i.is_equal(index1,value1), true);
  EXPECT_EQ(et_i.is_equal(index2,value2), true);
  EXPECT_EQ(et_i.is_equal(index3,value3), true);
  et_i.unset_each();
  EXPECT_EQ(et_i.is_equal(index1,value1), false);
  EXPECT_EQ(et_i.is_equal(index2,value2), false);
  EXPECT_EQ(et_i.is_equal(index3,value3), false);
}

TEST_F(EntityTraitTestingF, SetAll)
{
  //set all values to all_value
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index
}

TEST_F(EntityTraitTestingF, SetAllSetTraitInteraction)
{
  //set all values to all_value and then test that set_trait changes flag
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index
  et_d.set_trait(index1, value1); //unset the all_value
  EXPECT_EQ(et_d.is_equal(index1,value1), true); //set correctly
  EXPECT_NE(et_d.is_equal(index3,all_value), true); //all_value is out of date
}

TEST_F(EntityTraitTestingF, SetAllUnsetTraitInteraction)
{
  //set all values to all_value and then test that unset_all changes flag
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index

  //unset_trait with value that is not in trait has not effect on set_all flag
  et_d.unset_trait(index1);
  EXPECT_EQ(et_d.is_equal(index2,all_value), true); //all_value still good

  //set some trait
  et_d.set_trait(index1, value1); //put something in the trait
  et_d.set_all(all_value); //set_all with something in the trait
  et_d.unset_trait(index1); //unset a value that's in the trait 
  EXPECT_NE(et_d.is_equal(index2,all_value), true); //all_value out of date
}

TEST_F(EntityTraitTestingF, SetAllUnsetAllInteraction)
{
  //set all values to all_value and then test that unset_all changes flag
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index
  et_d.unset_all();
  EXPECT_NE(et_d.is_equal(index2,all_value), true); //all_value is out of date

  //set some trait
  et_d.set_trait(index1,value1);
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index
  et_d.unset_all();
  EXPECT_NE(et_d.is_equal(index2,all_value), true); //all_value is out of date
  EXPECT_EQ(et_d.is_equal(index1,value1), true); //trait existed before set_all
}

TEST_F(EntityTraitTestingF, SetAllUnsetEachInteraction)
{
  //set all values to all_value and then test that unset_each changes flag
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index
  et_d.unset_each();
  EXPECT_NE(et_d.is_equal(index2,all_value), true); //all_value is out of date

  //set some trait
  et_d.set_trait(index1,value1);
  et_d.set_all(all_value);
  EXPECT_EQ(et_d.is_equal(index1,value1), false); //value1 != all_value
  EXPECT_EQ(et_d.is_equal(index3,all_value), true); //all_value for any index
  et_d.unset_each();
  EXPECT_NE(et_d.is_equal(index2,all_value), true); //all_value is out of date
  EXPECT_NE(et_d.is_equal(index1,value1), true); //traits cleared
}
