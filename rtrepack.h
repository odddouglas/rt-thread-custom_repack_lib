/*
 * Copyright (c) 2024 odddouglas
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-12     odddouglas   the first version
 */

#ifndef __RT_REPACK_H__
#define __RT_REPACK_H__

#include <rtdbg.h>
#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#include <rtdef.h>
#include <rtconfig.h>

/**
 * @brief  创建或初始化一个信号量，支持动态和静态创建。
 *
 * @param[in,out]  sem            指向要创建或初始化的信号量控制块的指针。
 *                                - 若 `is_dynamic` 为 `RT_FALSE`（静态创建），
 *                                  则需传入已分配的信号量控制块的地址。可定义全局：`struct rt_semaphore sem;`
 *                                - 若 `is_dynamic` 为 `RT_TRUE`（动态创建），
 *                                  则传入 一个值`RT_NULL`的指针即可，内核将动态分配空间。可定义全局：`rt_sem_t sem = RT_NULL;`
 * @param[in]      name           信号量的名称字符串。
 * @param[in]      initial_value  信号量的初始计数值。
 * @param[in]      flag           信号量创建的标志位，定义信号量的行为特性：
 *                                - `RT_IPC_FLAG_FIFO`：先进先出方式
 *                                - `RT_IPC_FLAG_PRIO`：优先级方式
 * @param[in]      is_dynamic     指示是否动态创建信号量。
 *                                - `RT_TRUE`：动态创建信号量，内核将分配内存。
 *                                - `RT_FALSE`：静态创建信号量，需提供有效的控制块地址。
 *
 * @return `RT_EOK` 表示成功，其他错误代码表示失败：
 *         - `-ENOMEM`：内存不足导致动态创建失败。
 *         - 非 `RT_EOK`：静态创建失败。
 *
 * @note  若使用动态创建信号量（`is_dynamic` 为 `RT_TRUE`），
 *        用户需在信号量不再使用时调用 `rt_sem_delete` 释放内存。
 *        而静态创建的信号量在使用完毕后调用 `rt_sem_detach`。
 */
rt_err_t semaphore_generate(struct rt_semaphore *sem,
                            const char *name,
                            rt_uint32_t initial_value,
                            rt_uint8_t flag,
                            rt_bool_t is_dynamic)
{
    if (is_dynamic)
    { // 动态创建
        sem = rt_sem_create(name, initial_value, flag);
        if (sem == RT_NULL)
        {
            LOG_E("rt_sem_create failed..\n");
            return -ENOMEM;
        }
        LOG_D("rt_sem_create sccessed...\n");
    }
    else
    {
        // 静态创建
        int ret = RT_EOK;
        ret = rt_sem_init(sem, name, initial_value, flag);
        if (ret != RT_EOK)
        {
            LOG_E("rt_sem_init failed...\n");
            return ret;
        }
        LOG_D("rt_sem_init sccessed...\n");
    }
    return RT_EOK;
}

#endif
