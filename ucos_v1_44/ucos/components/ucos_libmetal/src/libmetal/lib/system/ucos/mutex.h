/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Xilinx nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file    ucos/mutex.h
 * @brief   uC/OS mutex primitives for libmetal.
 */

#ifndef __METAL_MUTEX__H__
#error "Include metal/mutex.h instead of metal/ucos/mutex.h"
#endif

#ifndef __METAL_UCOS_MUTEX__H__
#define __METAL_UCOS_MUTEX__H__

#include <errno.h>
#include <Source/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (OS_VERSION >= 30000u)
typedef OS_MUTEX metal_mutex_t;
#else
typedef struct {
    INT8U      prio;
    OS_EVENT  *os_mutex;
} metal_mutex_t;
#endif

#define METAL_MUTEX_INIT        { NULL }

static inline void metal_mutex_init(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  p_err;


    OSMutexCreate((OS_MUTEX *)mutex, (CPU_CHAR *)0, &p_err);
#else
    INT8U  perr;


    mutex->os_mutex = OSMutexCreate(mutex->prio, &perr);
#endif
}

static inline void metal_mutex_deinit(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  p_err;


    OSMutexDel((OS_MUTEX *)mutex, OS_OPT_DEL_ALWAYS, &p_err);
#else
    INT8U  perr;


    OSMutexDel(mutex->os_mutex, OS_DEL_ALWAYS, &perr);
#endif
}

static inline int metal_mutex_try_acquire(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  p_err;


    OSMutexPend((OS_MUTEX *)mutex, 0, OS_OPT_PEND_NON_BLOCKING, (CPU_TS *)0, &p_err);

    if ((p_err == OS_ERR_NONE       ) ||
        (p_err == OS_ERR_MUTEX_OWNER)) {
        return 1;
    }

    return 0;
#else
    (void)mutex;

    return -ENOSYS;                                             /* No function to do this in uCOS-II.                   */
#endif

}

static inline void metal_mutex_acquire(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  p_err;


    OSMutexPend((OS_MUTEX *)mutex, 0, OS_OPT_PEND_BLOCKING, (CPU_TS *)0, &p_err);
#else
    INT8U  perr;


    OSMutexPend(mutex->os_mutex, 0, &perr);
#endif
}

static inline void metal_mutex_release(metal_mutex_t *mutex)
{
#if (OS_VERSION >= 30000u)
    OS_ERR  p_err;


    OSMutexPost((OS_MUTEX *)mutex, OS_OPT_POST_NONE, &p_err);
#else
    OSMutexPost(mutex->os_mutex);
#endif
}

static inline int metal_mutex_is_acquired(metal_mutex_t *mutex)
{
    int isAcquired;
#if (OS_VERSION >= 30000u)
    CPU_SR_ALLOC();

                                                                /* This hack is necessary because of the uC/OS-III API. */
    CPU_CRITICAL_ENTER();
    isAcquired = (((OS_MUTEX *)mutex)->OwnerNestingCtr == 0) ? 0 : 1;
    CPU_CRITICAL_EXIT();
#else
    OS_MUTEX_DATA  p_mutex_data;


    OSMutexQuery (mutex->os_mutex, &p_mutex_data);
    isAcquired = (p_mutex_data.OSValue == OS_FALSE) ? 1 : 0;
#endif

    return isAcquired;
}

#ifdef __cplusplus
}
#endif

#endif /* __METAL_UCOS_MUTEX__H__ */
