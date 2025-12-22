#pragma once
#define CONSTEXPR constexpr

#undef  PI
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

namespace SDK
{
	class FMath {
      public:
            template <class T>
            static CONSTEXPR FORCEINLINE T Min( const T A, const T B ) {
                    return ( A <= B ) ? A : B;
            }

	        static FORCEINLINE float Sin(float Value) { return sinf(Value); }
	        static FORCEINLINE float Cos(float Value) { return cosf(Value); }

            template <class T>
            static constexpr FORCEINLINE T
            DivideAndRoundUp( T Dividend, T Divisor ) {
                    return ( Dividend + Divisor - 1 ) / Divisor;
            }

            static FORCEINLINE UINT32 FloorLog2( UINT32 Value ) {
                    // Use BSR to return the log2 of the integer
                    unsigned long Log2;
                    if ( _BitScanReverse( &Log2, Value ) != 0 ) {
                            return Log2;
                    }

                    return 0;
            }
    };
}