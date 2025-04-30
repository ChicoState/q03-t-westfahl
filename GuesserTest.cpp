/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

//Match on 1st try. Basic Test
TEST(GuesserTest, match_test_1)
{
  Guesser object("Secret");
  ASSERT_EQ(true, object.match("Secret"));
}

//Match on 3rd try
TEST(GuesserTest, match_test_2)
{
  Guesser object("Secret");
  ASSERT_EQ(false, object.match("secret"));
  ASSERT_EQ(false, object.match("Secrets"));
  ASSERT_EQ(true, object.match("Secret"));
}

//Match on 1st try for remaining check
TEST(GuesserTest, remaining_test_1)
{
  Guesser object("Secret");
  ASSERT_EQ(3, object.remaining());

  object.match("Secret");
  ASSERT_EQ(3, object.remaining());
}

//Match on 3rd try for remaining check
TEST(GuesserTest, remaining_test_2)
{
  Guesser object("Secret");
  ASSERT_EQ(3, object.remaining());

  object.match("secret");
  ASSERT_EQ(2, object.remaining());

  object.match("Secrets");
  ASSERT_EQ(1, object.remaining());

  object.match("Secret");
  ASSERT_EQ(3, object.remaining()); //pretty sure this line discovered a bug
}

//Re-run, a series of correct guesses
TEST(GuesserTest, re_run)
{
  Guesser object("Many such cases");

  ASSERT_EQ(false, object.match("Many such case"));
  ASSERT_EQ(false, object.match("Many such cAses"));
  ASSERT_EQ(true, object.match("Many such cases"));

  //Number of tries should reset to three and exact same scenario should be possible
  ASSERT_EQ(false, object.match("Many such case"));
  ASSERT_EQ(false, object.match("Many such cAses"));
  ASSERT_EQ(true, object.match("Many such cases"));
}

//1 wrong guess. Tests to see if both match() and remainig() work
TEST(GuesserTest, baisc_wrong_test)
{
  Guesser object("Secret");
  ASSERT_EQ(3, object.remaining());

  ASSERT_EQ(false, object.match("secret"));
  ASSERT_EQ(2, object.remaining());
}

//Brute Force lockout
TEST(GuesserTest, brute_force_lockout)
{
  Guesser object("brute");
  ASSERT_EQ(false, object.match("brute_simpleton"));
  ASSERT_EQ(false, object.match("brute"));
}

//Remaining variable test after lockout. Should countdown as usual to hide fact that it is locked
TEST(GuesserTest, post_lockout_countdown)
{
  Guesser object("hello");
  object.remaining();

  object.match("hello world");
  ASSERT_EQ(2, object.remaining());

  object.match("hello");
  ASSERT_EQ(1, object.remaining());
}

//Attempt Lockout > 3 attempts
TEST(GuesserTest, attempt_lockout)
{
  Guesser object("Scary");
  ASSERT_EQ(3, object.remaining());

  ASSERT_EQ(false, object.match("Scare"));
  ASSERT_EQ(2, object.remaining());

  ASSERT_EQ(false, object.match("scary"));
  ASSERT_EQ(1, object.remaining());

  ASSERT_EQ(false, object.match("Scared"));
  ASSERT_EQ(0, object.remaining());

  ASSERT_EQ(false, object.match("Scary"));
  ASSERT_EQ(0, object.remaining());
}

//32 Character cuttoff test
TEST(GuesserTest, 32CharacterTest)
{
  Guesser object("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ"); //>32 Characters
  ASSERT_EQ(true, object.match("abcdefghijklmnopqrstuvwxyz ABCDE")); //=32 Characters
}

//Weird String
TEST(GuesserTest, weird_string)
{
  Guesser object("  +=5g ao4<./i2jw @3$*5  21");
  ASSERT_EQ(false, object.match("  +=5g ao4<./i2jw @3$*5  22")); //1 character difference
  ASSERT_EQ(true, object.match("  +=5g ao4<./i2jw @3$*5  21"));
}