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
extern void Rainbow();
extern void SmileFace();
extern void Snake_Basic(int Bri);
extern void Snake_Loop(int Bri);
extern void Snake_Single(int Bri);
extern void Snake_Single_withLP5860(int Bri);
extern void Helix_Single(int Bri);
extern void Helix_Night(int Bri);
extern void Firework(int Bri);
extern void Firework_Night(int Bri);
extern void Rainbow_test();

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
