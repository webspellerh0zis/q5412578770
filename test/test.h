﻿
extern int tests_count;

#define TEST_LOG(format, ...)\
Logger_Log("[test] %s(): "format, __FUNCTION__ , ##__VA_ARGS__)

#define PRINT_TEST_RESULT(N)\
Logger_Log("[test] %d tests, %d pass.\n", tests_count, tests_count + N)

#define CHECK(X) do {\
	tests_count += 1;\
	LOG("[test] %s(): %s. # %s\n", __FUNCTION__, ""#X"",\
	    (X) ? "PASS" : (ret -= 1, "NO PASS!"));\
} while( 0 );

#define CHECK2(X) do {\
	if( !(X) ) {\
		LOG( "[test] %s(): %s. # NO PASS!\n", __FUNCTION__, ""#X"" );\
		ret -= 1;\
	}\
} while( 0 );

int test_string( void );
int test_css_parser( void );
int test_char_render( void );
int test_string_render( void );
int test_widget_render( void );
int test_widget_layout( void );
int test_widget_rect( void );
int test_image_reader( void );
