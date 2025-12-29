#include "gtest.h"
#include "ring_buffer.h"
#include <string.h>

#define CONFIG_BUFFER_LEN	(256)
static rb_t rb;
static unsigned char buff[CONFIG_BUFFER_LEN];

TEST(rb_init, basic)
{
	ASSERT_EQ(0, rbInit(&rb, buff, CONFIG_BUFFER_LEN));

	EXPECT_EQ(0, rb.in);
	EXPECT_EQ(0, rb.out);
	EXPECT_EQ(CONFIG_BUFFER_LEN, rb.size);
	EXPECT_EQ(buff, rb.buffer);
	EXPECT_TRUE(rbIsEmpty(&rb));
	EXPECT_FALSE(rbIsFull(&rb));
}

TEST(rb_put, put_hello_001)
{
	char str[] = "hellohello";
	unsigned int len = strlen(str);
	
	EXPECT_EQ(len, rbPut(&rb, (unsigned char*)str, len));

	EXPECT_EQ(len, strlen(str));
	EXPECT_EQ(0, rb.out);
	EXPECT_EQ(len, rb.in);
	EXPECT_EQ(len, rbUsed(&rb));
	EXPECT_EQ(CONFIG_BUFFER_LEN-len, rbUnUsed(&rb));
	EXPECT_FALSE(rbIsEmpty(&rb)) << "ring buffer used: " << rbUsed(&rb);
}

TEST(rb_put, put_hello_002)
{
	char str[] = "hellohello";
	unsigned int len = strlen(str);
	
	EXPECT_EQ(len, rbPut(&rb, (unsigned char*)str, len));

	EXPECT_EQ(len, strlen(str));
	EXPECT_EQ(len<<1, rbUsed(&rb));
	EXPECT_EQ(CONFIG_BUFFER_LEN-(len<<1), rbUnUsed(&rb));
	EXPECT_FALSE(rbIsEmpty(&rb)) << "ring buffer used: " << rbUsed(&rb);
}

TEST(rb_get, gut_hello_001)
{
	char str[CONFIG_BUFFER_LEN];
	unsigned int len = strlen("hello");

	ASSERT_EQ(5, len);
	EXPECT_EQ(len, rbGet(&rb, (unsigned char*)str, len));
	EXPECT_EQ(0, memcmp(str, "hello", len));
	EXPECT_EQ(5, rb.out);
	EXPECT_FALSE(rbIsEmpty(&rb)) << "ring buffer used: " << rbUsed(&rb);
}

TEST(rb_get, gut_hello_002)
{
	char str[CONFIG_BUFFER_LEN];
	unsigned int len = strlen("hello");

	ASSERT_EQ(5, len);
	EXPECT_EQ(len, rbGet(&rb, (unsigned char*)str, len));
	EXPECT_EQ(0, memcmp(str, "hello", len));
	EXPECT_EQ(10, rb.out);
	EXPECT_FALSE(rbIsEmpty(&rb)) << "ring buffer used: " << rbUsed(&rb);
}

TEST(rb_deinit, deinit)
{
	rbDeInit(&rb);

	EXPECT_EQ(0, rb.in);
	EXPECT_EQ(0, rb.out);
}

TEST(rb_full, full_001)
{
	unsigned char buf[CONFIG_BUFFER_LEN];

	for(int i=0; i<CONFIG_BUFFER_LEN; i++){
		buf[i] = i;
	}

	unsigned int len = 2;
	unsigned char buf2[CONFIG_BUFFER_LEN];

	ASSERT_EQ(0, rbInit(&rb, buff, CONFIG_BUFFER_LEN));
	ASSERT_EQ(CONFIG_BUFFER_LEN, rbPut(&rb, buf, CONFIG_BUFFER_LEN));
	EXPECT_TRUE(rbIsFull(&rb));
	EXPECT_EQ(CONFIG_BUFFER_LEN, rbUsed(&rb));
	ASSERT_EQ(256, rb.in);
	ASSERT_EQ(0, rb.out);
	//ASSERT_EQ(len, rbPut(&rb, buf, len)); // full 时不覆盖
	
	EXPECT_EQ(10, rbGet(&rb, buf2, 10));
	EXPECT_EQ(CONFIG_BUFFER_LEN - 10, rbUsed(&rb));
	EXPECT_EQ(0, memcmp(buf, buf2, 10));
}

TEST(rb_putget, putget_001)
{
	unsigned char buf[CONFIG_BUFFER_LEN];

	for(int i=0; i<CONFIG_BUFFER_LEN; i++){
		buf[i] = i;
	}

	ASSERT_EQ(0, rbInit(&rb, buff, CONFIG_BUFFER_LEN));
	EXPECT_EQ(10, rbPut(&rb, buf, 10));
	EXPECT_EQ(10, rbGet(&rb, buf, 11));
	EXPECT_TRUE(rbIsEmpty(&rb));
}

TEST(rb_putput, putput_001)
{
	unsigned char buf[CONFIG_BUFFER_LEN];

	for(int i=0; i<CONFIG_BUFFER_LEN; i++){
		buf[i] = i;
	}

	ASSERT_EQ(0, rbInit(&rb, buff, CONFIG_BUFFER_LEN));
	EXPECT_EQ(CONFIG_BUFFER_LEN - 50, rbPut(&rb, buf, CONFIG_BUFFER_LEN - 50));
	EXPECT_EQ(50, rbPut(&rb, buf, CONFIG_BUFFER_LEN));
	EXPECT_FALSE(rbIsEmpty(&rb));
}

TEST(rb_putget, putget003)
{
	unsigned char buf[CONFIG_BUFFER_LEN];
	unsigned char buf2[CONFIG_BUFFER_LEN];

	for(int i=0; i<CONFIG_BUFFER_LEN; i++)
		buf2[i] = i;

	ASSERT_EQ(0, rbInit(&rb, buff, CONFIG_BUFFER_LEN));
	EXPECT_EQ(246, rbPut(&rb, buf2, 246));

	memset(buf2, 0, CONFIG_BUFFER_LEN);
	EXPECT_EQ(10, rbGet(&rb, buf2, 10));
	EXPECT_EQ(236, rbUsed(&rb));

	memset(buf2, 0xAA, CONFIG_BUFFER_LEN);
	EXPECT_EQ(15, rbPut(&rb, buf2, 15));

	memset(buf2, 0, CONFIG_BUFFER_LEN);
	EXPECT_EQ(236 + 15, rbGet(&rb, buf2, rbUsed(&rb)));
	EXPECT_EQ(10, buf2[0]);
	EXPECT_EQ(11, buf2[1]);
	EXPECT_EQ(0xAA, buf2[236 + 15 -1]);
	EXPECT_EQ(0xAA, buf2[236 + 15 -2]);
	EXPECT_EQ(0xAA, buf2[236 + 15 -10]);
	EXPECT_EQ(0xAA, buf2[236 + 15 -15]);
	EXPECT_NE(0xAA, buf2[236 + 15 -16]);

	EXPECT_EQ(0, rbUsed(&rb));
}
