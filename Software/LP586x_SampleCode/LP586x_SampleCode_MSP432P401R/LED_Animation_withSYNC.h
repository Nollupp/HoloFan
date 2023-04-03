#ifndef LED_ANIMATION_H_
#define LED_ANIMATION_H_
//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

extern void Breathing();
extern void Chasing();
extern void Rainbow_test();
extern void Count(int Bri,
                  int R,
                  int G,
                  int B)
extern void Animation_Start();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif /* LED_ANIMATION_H_ */
