#include "icaro.hpp"
#include <cstdio>


auto main( int argc, char* argv[] ) -> int
{
    TEST( verify_true,
    {
        VERIFY( true );
        return true;
    });

    TEST( verify_eq,
    {
        auto var_1 = 43;
        auto var_2 = 42;
        VERIFY_EQ( var_1, var_2 );
        return true;
    });

    TEST( verify_not_eq,
    {
        auto var_1 = 67;
        auto var_2 = 69;
        VERIFY_NOT_EQ( var_1, var_2 );
        return true;
    });

    printf( "# TESTING VERIFY\n" );
    Icaro::run({});
    printf( "\n---\n\n" );

    printf( "# TESTING NAME FILTER 'eq'\n" );
    Icaro::run({ .filter = "eq" });
    printf( "\n---\n\n" );

    printf( "# TESTING SETUP AND TEARDOWN\n" );
    {
        auto setup =    []( void** context ) -> void { printf( "🏗️ SETUP\n"    ); };
        auto teardown = []( void** context ) -> void { printf( "⚒️ TEARDOWN\n" ); };
        Icaro::run({ .setup = setup, .teardown = teardown });
    }
    printf( "\n---\n\n" );

    printf( "# TESTING SHADER CONTEXT\n" );
    {
        static constexpr int item_count = 4;

        auto allocate_ctx = []( void** context ) -> void
        {
            *context = new int[ item_count ];
            for( int i=0; i<item_count; ++i )
                ((int*)(*context))[i] = i;
        };

        auto deallocate_ctx = []( void** context ) -> void
        {
            delete[] (int*)(*context);
            *context = nullptr;
        };

        TEST( shader_context,
        {
            VERIFY_PTR( ctx );
            for( int i=0; i<item_count; ++i )
            {
                VERIFY_EQ( ((int*)ctx)[i], i );
            }
            return true;
        });
        Icaro::run({ .filter = "shader_context", .setup = allocate_ctx, .teardown = deallocate_ctx });
    }
    printf( "\n---\n\n" );
}
