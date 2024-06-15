/* superlu_dist_config.h.in */

/* Enable CUDA */
/* #undef HAVE_CUDA */

/* Enable HIP */
/* #undef HAVE_HIP */

/* Enable parmetis */
#define HAVE_PARMETIS TRUE

/* Enable colamd */
#define HAVE_COLAMD TRUE

/* Enable LAPACK */
#define SLU_HAVE_LAPACK TRUE

/* Enable CombBLAS */
/* #undef HAVE_COMBBLAS */

/* enable 64bit index mode */
/* #undef XSDK_INDEX_SIZE */

#if defined(XSDK_INDEX_SIZE) && (XSDK_INDEX_SIZE == 64)
#define _LONGINT 1
#endif
