/* defines for debugging */
#define DEBUG_OFF           99
#define DEBUG_BASIC         03 
#define DEBUG_ADV           02
#define DEBUG_EXP           01

static int can_debug = DEBUG_EXP;

// debug version
#ifdef _DEBUG

    #define DEBUG(x, outp)        ( dbg_p(x, outp ) )

    _inline void dbg_p (char i, char *fmt, ...)
    {
      char tmp[100];
      if (i>=can_debug){

        sprintf (tmp, "xlCANcontrol: %s", fmt);

      // OutputDebugString (tmp);
      }
    } 
    
// release version.
#else
    #define DEBUG(x, outp)     
#endif