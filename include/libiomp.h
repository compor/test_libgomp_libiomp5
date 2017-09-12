#ifndef libiomp_h_INCLUDED
#define libiomp_h_INCLUDED

#define KMP_EXPORT     extern  /* export declaration in guide libraries */

typedef int kmp_int32;
typedef void (*kmpc_micro)              ( kmp_int32 * global_tid, kmp_int32 * bound_tid, ... );

typedef kmp_int32 kmp_critical_name[8];

/*! Use trampoline for internal microtasks */
#define KMP_IDENT_IMB             0x01
/*! Use c-style ident structure */
#define KMP_IDENT_KMPC            0x02
/*! Entry point generated by auto-parallelization */
#define KMP_IDENT_AUTOPAR         0x08
/*! Compiler generates atomic reduction option for kmpc_reduce* */
#define KMP_IDENT_ATOMIC_REDUCE   0x10
/*! To mark a 'barrier' directive in user code */
#define KMP_IDENT_BARRIER_EXPL    0x20
/*! To Mark implicit barriers. */
#define KMP_IDENT_BARRIER_IMPL           0x0040
#define KMP_IDENT_BARRIER_IMPL_MASK      0x01C0
#define KMP_IDENT_BARRIER_IMPL_FOR       0x0040
#define KMP_IDENT_BARRIER_IMPL_SECTIONS  0x00C0

#define KMP_IDENT_BARRIER_IMPL_SINGLE    0x0140
#define KMP_IDENT_BARRIER_IMPL_WORKSHARE 0x01C0
enum sched_type {
    kmp_sch_lower                     = 32,   /**< lower bound for unordered values */
    kmp_sch_static_chunked            = 33,
    kmp_sch_static                    = 34,   /**< static unspecialized */
    kmp_sch_dynamic_chunked           = 35,
    kmp_sch_guided_chunked            = 36,   /**< guided unspecialized */
    kmp_sch_runtime                   = 37,
    kmp_sch_auto                      = 38,   /**< auto */
    kmp_sch_trapezoidal               = 39,

    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_sch_static_greedy             = 40,
    kmp_sch_static_balanced           = 41,
    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_sch_guided_iterative_chunked  = 42,
    kmp_sch_guided_analytical_chunked = 43,

    kmp_sch_static_steal              = 44,   /**< accessible only through KMP_SCHEDULE environment variable */

#if OMP_45_ENABLED
    kmp_sch_static_balanced_chunked   = 45,   /**< static with chunk adjustment (e.g., simd) */
#endif

    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_sch_upper                     = 46,   /**< upper bound for unordered values */

    kmp_ord_lower                     = 64,   /**< lower bound for ordered values, must be power of 2 */
    kmp_ord_static_chunked            = 65,
    kmp_ord_static                    = 66,   /**< ordered static unspecialized */
    kmp_ord_dynamic_chunked           = 67,
    kmp_ord_guided_chunked            = 68,
    kmp_ord_runtime                   = 69,
    kmp_ord_auto                      = 70,   /**< ordered auto */
    kmp_ord_trapezoidal               = 71,
    kmp_ord_upper                     = 72,   /**< upper bound for ordered values */

#if OMP_40_ENABLED
    /* Schedules for Distribute construct */
    kmp_distribute_static_chunked     = 91,   /**< distribute static chunked */
    kmp_distribute_static             = 92,   /**< distribute static unspecialized */
#endif

    /*
     * For the "nomerge" versions, kmp_dispatch_next*() will always return
     * a single iteration/chunk, even if the loop is serialized.  For the
     * schedule types listed above, the entire iteration vector is returned
     * if the loop is serialized.  This doesn't work for gcc/gcomp sections.
     */
    kmp_nm_lower                      = 160,  /**< lower bound for nomerge values */

    kmp_nm_static_chunked             = (kmp_sch_static_chunked - kmp_sch_lower + kmp_nm_lower),
    kmp_nm_static                     = 162,  /**< static unspecialized */
    kmp_nm_dynamic_chunked            = 163,
    kmp_nm_guided_chunked             = 164,  /**< guided unspecialized */
    kmp_nm_runtime                    = 165,
    kmp_nm_auto                       = 166,  /**< auto */
    kmp_nm_trapezoidal                = 167,

    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_nm_static_greedy              = 168,
    kmp_nm_static_balanced            = 169,
    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_nm_guided_iterative_chunked   = 170,
    kmp_nm_guided_analytical_chunked  = 171,
    kmp_nm_static_steal               = 172,  /* accessible only through OMP_SCHEDULE environment variable */

    kmp_nm_ord_static_chunked         = 193,
    kmp_nm_ord_static                 = 194,  /**< ordered static unspecialized */
    kmp_nm_ord_dynamic_chunked        = 195,
    kmp_nm_ord_guided_chunked         = 196,
    kmp_nm_ord_runtime                = 197,
    kmp_nm_ord_auto                   = 198,  /**< auto */
    kmp_nm_ord_trapezoidal            = 199,
    kmp_nm_upper                      = 200,  /**< upper bound for nomerge values */

#if OMP_45_ENABLED
    /* Support for OpenMP 4.5 monotonic and nonmonotonic schedule modifiers.
     * Since we need to distinguish the three possible cases (no modifier, monotonic modifier,
     * nonmonotonic modifier), we need separate bits for each modifier.
     * The absence of monotonic does not imply nonmonotonic, especially since 4.5 says
     * that the behaviour of the "no modifier" case is implementation defined in 4.5,
     * but will become "nonmonotonic" in 5.0.
     *
     * Since we're passing a full 32 bit value, we can use a couple of high bits for these
     * flags; out of paranoia we avoid the sign bit.
     *
     * These modifiers can be or-ed into non-static schedules by the compiler to pass
     * the additional information.
     * They will be stripped early in the processing in __kmp_dispatch_init when setting up schedules, so
     * most of the code won't ever see schedules with these bits set.
     */
    kmp_sch_modifier_monotonic      = (1<<29), /**< Set if the monotonic schedule modifier was present */
    kmp_sch_modifier_nonmonotonic   = (1<<30), /**< Set if the nonmonotonic schedule modifier was present */

# define SCHEDULE_WITHOUT_MODIFIERS(s) (enum sched_type)((s) & ~ (kmp_sch_modifier_nonmonotonic | kmp_sch_modifier_monotonic))
# define SCHEDULE_HAS_MONOTONIC(s)     (((s) & kmp_sch_modifier_monotonic)    != 0)
# define SCHEDULE_HAS_NONMONOTONIC(s)  (((s) & kmp_sch_modifier_nonmonotonic) != 0)
# define SCHEDULE_HAS_NO_MODIFIERS(s)  (((s) & (kmp_sch_modifier_nonmonotonic | kmp_sch_modifier_monotonic)) == 0)
#else
    /* By doing this we hope to avoid multiple tests on OMP_45_ENABLED. Compilers can now eliminate tests on compile time
     * constants and dead code that results from them, so we can leave code guarded by such an if in place.
     */
# define SCHEDULE_WITHOUT_MODIFIERS(s) (s)
# define SCHEDULE_HAS_MONOTONIC(s)     false
# define SCHEDULE_HAS_NONMONOTONIC(s)  false
# define SCHEDULE_HAS_NO_MODIFIERS(s)  true
#endif

    kmp_sch_default = kmp_sch_static  /**< default scheduling algorithm */
};
typedef struct ident {
	kmp_int32 reserved_1;   /**<  might be used in Fortran; see above  */
	kmp_int32 flags;        /**<  also f.flags; KMP_IDENT_xxx flags; KMP_IDENT_KMPC identifies this union member  */
	kmp_int32 reserved_2;   /**<  not really used in Fortran any more; see above */
	kmp_int32 reserved_3;   /**<  source[4] in Fortran, do not use for C++  */
	char const *psource;    /**<  String describing the source location.
                                      The string is composed of semi-colon separated fields which describe the source file,
                                      the function and a pair of line numbers that delimit the construct.
                                */
} ident_t;

KMP_EXPORT kmp_int32  __kmpc_global_thread_num  ( ident_t * );
KMP_EXPORT void   __kmpc_fork_call            ( ident_t *, kmp_int32 nargs, kmpc_micro microtask, ... );
KMP_EXPORT void   __kmpc_begin                ( ident_t *, kmp_int32 flags );
KMP_EXPORT void   __kmpc_end                  ( ident_t * );
KMP_EXPORT void __kmpc_barrier(ident_t *loc, kmp_int32 global_tid);
KMP_EXPORT void __kmpc_for_static_init_4(ident_t *loc, kmp_int32 gtid, kmp_int32 schedtype, kmp_int32 *plastiter,
                            kmp_int32 *plower, kmp_int32 *pupper, kmp_int32 *pstride, kmp_int32 incr,
                            kmp_int32 chunk);

KMP_EXPORT void __kmpc_for_static_fini(ident_t *loc, kmp_int32 global_tid);

KMP_EXPORT void __kmpc_dispatch_init_4( ident_t *loc, kmp_int32 gtid,
    enum sched_type schedule, kmp_int32 lb, kmp_int32 ub, kmp_int32 st,
    kmp_int32 chunk );

KMP_EXPORT void __kmpc_dispatch_fini_4( ident_t *loc, kmp_int32 gtid );
KMP_EXPORT int __kmpc_dispatch_next_4( ident_t *loc, kmp_int32 gtid,
    kmp_int32 *p_last, kmp_int32 *p_lb, kmp_int32 *p_ub, kmp_int32 *p_st );

/*
 * Interface to fast scalable reduce methods routines
 */

KMP_EXPORT kmp_int32 __kmpc_reduce_nowait( ident_t *loc, kmp_int32 global_tid,
                                           kmp_int32 num_vars, size_t reduce_size,
                                           void *reduce_data, void (*reduce_func)(void *lhs_data, void *rhs_data),
                                           kmp_critical_name *lck );
KMP_EXPORT void __kmpc_end_reduce_nowait( ident_t *loc, kmp_int32 global_tid, kmp_critical_name *lck );
KMP_EXPORT kmp_int32 __kmpc_reduce( ident_t *loc, kmp_int32 global_tid,
                                    kmp_int32 num_vars, size_t reduce_size,
                                    void *reduce_data, void (*reduce_func)(void *lhs_data, void *rhs_data),
                                    kmp_critical_name *lck );
KMP_EXPORT void __kmpc_end_reduce( ident_t *loc, kmp_int32 global_tid, kmp_critical_name *lck );

KMP_EXPORT kmp_int32  __kmpc_single         ( ident_t *, kmp_int32 global_tid );
KMP_EXPORT void   __kmpc_end_single         ( ident_t *, kmp_int32 global_tid );

KMP_EXPORT void   __kmpc_critical           ( ident_t *, kmp_int32 global_tid, kmp_critical_name * );
KMP_EXPORT void   __kmpc_end_critical       ( ident_t *, kmp_int32 global_tid, kmp_critical_name * );

// 4-byte add / sub fixed
void __kmpc_atomic_fixed4_add(  ident_t *id_ref, int gtid, kmp_int32 * lhs, kmp_int32 rhs );
void __kmpc_atomic_fixed4_sub(  ident_t *id_ref, int gtid, kmp_int32 * lhs, kmp_int32 rhs );

#endif // libiomp_h_INCLUDED

