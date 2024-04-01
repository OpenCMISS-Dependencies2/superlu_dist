/* superlu_dist_config.h.in */

/* Enable CUDA */
#define HAVE_CUDA TRUE

/* Enable NVSHMEM */
#define HAVE_NVSHMEM TRUE

/* Enable HIP */
/* #undef HAVE_HIP */

/* Enable parmetis */
#define HAVE_PARMETIS TRUE

/* Enable colamd */
/* #undef HAVE_COLAMD */

/* Enable LAPACK */
/* #undef SLU_HAVE_LAPACK */

/* Enable CombBLAS */
/* #undef HAVE_COMBBLAS */

/* Enable MAGMA */
#define HAVE_MAGMA TRUE

/* enable 64bit index mode */
/* #undef XSDK_INDEX_SIZE */

#if defined(XSDK_INDEX_SIZE) && (XSDK_INDEX_SIZE == 64)
#define _LONGINT 1
#endif
