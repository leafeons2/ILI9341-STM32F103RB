/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      shell.c
 * @brief     shell source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "shell.h"

static shell_t gs_shell;        /**< shell handle */

/**
 * @brief      shell pretreatment
 * @param[in]  *buf pointer to an in data buffer
 * @param[in]  in_len length of in data
 * @param[out] *buf_out pointer to an out data buffer
 * @param[out] *out_len pointer to the length of a out data buffer
 * @param[out] **argv pointer to a argv buffer
 * @param[out] *argc pointer to a argc buffer
 * @return     status code
 *             - 0 success
 *             - 1 pretreatment failed
 * @note       none
 */
static uint8_t a_shell_pretreatment(char *buf, uint16_t in_len, char *buf_out, uint16_t *out_len, char **argv, uint8_t *argc)
{
    uint16_t i;
    uint8_t flag = 0;
  
    *out_len = 0;
    *argc = 0;
    flag = 1;
    for (i = 0; i < in_len; i++)
    {
        if (buf[i] != ' ')
        {
            buf_out[(*out_len)] = buf[i];
            (*out_len)++;
            if (flag == 1)
            {
                argv[(*argc)] = &buf_out[(*out_len-1)];
                (*argc)++; 
                if((*argc) >= SHELL_MAX_SIZE)
                {
                    return 1;
                }
            }
            flag = 0;
            /*
             *
             *  -out_len 은 문자열들의 종합 길이이기도 하지만, 일시적으로 버퍼 인덱스로 쓰이기도 한다
             *
             *  -만약 띄어쓰기가 아니라면, 현재 문자를 버퍼의 인덱스가 가리키는 버퍼부분에 문자를 그대로 옮김.
             *   만약 이게 첫 글자라면, 현재 글자를 쓴 위치가 첫 글자보다 한 칸 앞의 인덱스이다.
             *   따라서 이것보다 한 칸 전 인덱스의 주소가 어떤 특정 문자열의 시작 주소이다.
             *   그 값을 문자열 포인터 들이 모인 argv 원소에 각각 저장한다
             */

        }
        else
        {
            if (flag == 0)
            {
                buf_out[(*out_len)] = (char)NULL;
                (*out_len)++;
                flag = 1;
            }

            /*
             *  - 만약 띄어쓰기가 맞고, 현재 문자열 작성중이었다면, buf_out에 띄어쓰기를 넣지 않고 NULL (\0) 를 넣음
             *    원래 함수는 그냥 NULL 을 넣지만, 나는 타입 오류가 떠서 (char)을 추가했다.
             */

        }
    }
    
    return 0;
}

/**
 * @brief     shell find function
 * @param[in] *handle pointer to a shell structure
 * @param[in] *name pointer to a name buffer
 * @return    status code
 *            - 0 success
 *            - 1 find failed
 * @note      none
 */
static uint8_t a_shell_find(shell_t *handle, char *name)
{
    uint16_t i;
    
    for (i = 0; i < SHELL_MAX_REG_SIZE; i++)
    {
       if (strcmp(handle->fuc[i].name, name) == 0)
       {
           return 0;
       }
    }
    
    return 1;
}

/**
 * @brief     shell run function
 * @param[in] *handle pointer to a shell structure
 * @param[in] argc number of param
 * @param[in] **argv param
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 find function failed
 * @note      none
 */
static uint8_t a_shell_run(shell_t *handle, uint8_t argc, char **argv)
{
    uint16_t i;
    
    for (i = 0; i < SHELL_MAX_REG_SIZE; i++)
    {
        if (strcmp(handle->fuc[i].name, argv[0]) == 0)
        {
            if (handle->fuc[i].fuc != NULL)
            {
                return handle->fuc[i].fuc(argc, argv);
            }
        }
    }
    
    return 2;
}

/**
 * @brief     shell register function
 * @param[in] *name pointer to a name buffer
 * @param[in] *fuc pointer to a function address
 * @return    status code
 *            - 0 success
 *            - 1 buffer is full
 * @note      none
 */
uint8_t shell_register(char *name, uint8_t (*fuc)(uint8_t argc, char **argv))
{
    if (gs_shell.fuc_i >= SHELL_MAX_REG_SIZE)
    {
        return 1;
    }
    
    strncpy(gs_shell.fuc[gs_shell.fuc_i].name, name, SHELL_MAX_NAME);
    gs_shell.fuc[gs_shell.fuc_i].fuc = fuc;
    gs_shell.fuc_i++;
    
    return 0;
}

/**
 * @brief  init shell
 * @return status code
 *         - 0 success
 * @note   none
 */
uint8_t shell_init(void)
{
    memset(&gs_shell, 0 , sizeof(shell_t));
    
    return 0;
}

/**
 * @brief     shell parse command
 * @param[in] *buf pointer to a buffer address
 * @param[in] len buffer length
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 find function failed
 *            - 3 length is too big
 *            - 4 pretreat failed
 * @note      none
 */
uint8_t shell_parse(char *buf, uint16_t len)
{
    uint8_t argc;
    uint16_t out_len;
    
    if (len > SHELL_MAX_BUF_SIZE)
    {
        return 3;
    }
    
    memset(&gs_shell.buf_out, 0 , sizeof(uint8_t) * SHELL_MAX_BUF_SIZE);
    //핸들 내 버퍼의 바이트들을 전부 0으로 초기화.

    if (a_shell_pretreatment(buf, len, gs_shell.buf_out, (uint16_t *)&out_len, gs_shell.argv, (uint8_t *)(&argc)) != 0)
    {
    	/*
    	 * buf_out에는 띄어쓰기를  '/0'로 대체한 문자열들이 들어오게 됨.
    	 * out_len 에는 문자열의 길이가 들어옴
    	 *
    	 * argv 는 소속된 각 문자열들의 시작주소들이 들어온다.
    	 * argc 는 문자열의 갯수가 담긴다.
    	 */
        return 4;
    }
    if (a_shell_find(&gs_shell, (char *)gs_shell.argv[0]) != 0)
    {
        return 2;
    } 
    
    return a_shell_run(&gs_shell, argc, (char **)gs_shell.argv);
}
