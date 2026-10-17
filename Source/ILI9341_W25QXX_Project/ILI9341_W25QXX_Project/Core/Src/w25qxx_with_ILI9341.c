
//user includes

#include "w25qxx_with_ILI9341.h"



// function definition


/**
 * @brief global var definition
 */

/*
uint8_t g_buf[256];                        // uart buffer
volatile uint16_t g_len;                   //< uart buffer length

*/

/**
 * @brief     w25qxx full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */



uint8_t w25qxx(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"data", required_argument, NULL, 2},
        {"dummy", required_argument, NULL, 3},
        {"length", required_argument, NULL, 4},
        {"interface", required_argument, NULL, 5},
        {"num", required_argument, NULL, 6},
        {"status", required_argument, NULL, 7},
        {"type", required_argument, NULL, 8},
        {"stream", no_argument, NULL, 9},
        {NULL, 0, NULL, 0},
    };
    char type[49] = "unknown";
    uint32_t addr = 0;
    uint32_t data = 0;
    w25qxx_qspi_read_dummy_t dummy = W25QXX_QSPI_READ_DUMMY_8_80MHZ;
    w25qxx_qspi_read_wrap_length_t length = W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE;
    w25qxx_interface_t interface = W25QXX_INTERFACE_SPI;
    w25qxx_security_register_t num = W25QXX_SECURITY_REGISTER_1;
    w25qxx_type_t chip_type = W25Q64;
    w25qxx_burst_wrap_t wrap = W25QXX_BURST_WRAP_NONE;
    uint8_t status;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 49);
                snprintf(type, 48, "t_%s", optarg);

                break;
            }

            /* addr */
            case 1 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                        /*
                         * 사용자가 적은 값이 0x 로 시작하는 주소가 맞다면,
                         * 문자열 중 0x로 시작하는 부분은 제외함.
                         */
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /*
                 * 문자열 형태의 0x 를 제외한 주소를 p에 담음
                 */

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)  // 첫 글자부터 해석 시작.
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }

                    /*
                     * 만약 9~0 사이 아스키 값이라면,  숫자 n 아스키 코드에  숫자 0 아스키 코드를 빼서 숫자로 만듬.
                     * 만약 F~A  f~a라면   알파벳 X 아스키 코드에서 알파벳 A 아스키 코드를 빼서, A로부터 얼마나
                     * 떨어져 있는지 확인한다.   A가 10이니, 차이와 10을 더해주면 해당 수를 만들 수 있다.
                     *
                     * 3글자의 숫자라면, 가장 큰 자리는 16^2 이므로, 처음으로 16의 제곱을 해주는 숫자는
                     * 글자수의 -1 으로 시작해야한다.   이후 반복에서는 반복 횟수만큼 1씩 더 차감
                     *
                     * 5가 반환되면 주소의 값에 포함될 수 없는 값이 넣어진 것.
                     */


                }

                /* set the addr */
                addr = hex_data & 0xFFFFFFFF;

                // 주소 범위를 넘어선 것은 아래 32비트만 살림.

                break;
            }

            /* data */
            case 2 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                // 16진수 데이터인지 점검하고, 16진수 특유의 기호를 뗌
                // 전달하는 데이터는 아래 번역 과정을 따른다면, 무조건 16진수로 보내야한다.

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the data */
                data = hex_data & 0xFFFFFFFF;

                break;
            }

            /* dummy */
            case 3 :
            {
                /* set the dummy */
                if (strcmp("DUMMY_2_33MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_2_33MHZ;
                }
                else if (strcmp("DUMMY_4_55MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_4_55MHZ;
                }
                else if (strcmp("DUMMY_6_80MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_6_80MHZ;
                }
                else if (strcmp("DUMMY_8_80MHZ", optarg) == 0)
                {
                    dummy = W25QXX_QSPI_READ_DUMMY_8_80MHZ;
                }
                else
                {
                    return 5;
                }
                break;
                // 통신에 필요한 dummy, 근데 dummy 가 전달되는게 안보이는데?
            }

            /* length */
            case 4 :
            {
                /* set the length */
                if (strcmp("8", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE;
                }
                else if (strcmp("16", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_16_BYTE;
                }
                else if (strcmp("32", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_32_BYTE;
                }
                else if (strcmp("64", optarg) == 0)
                {
                    length = W25QXX_QSPI_READ_WRAP_LENGTH_64_BYTE;
                }
                else
                {
                    return 5;
                }

                break;

                // 길이는 "8" / 16 / 32 / "64" 의 문자열만 오도록 예정

            }

            /* interface */
            case 5 :
            {
                /* set the interface */
                if (strcmp("spi", optarg) == 0)
                {
                    interface = W25QXX_INTERFACE_SPI;
                }
                else if (strcmp("qspi", optarg) == 0)
                {
                    interface = W25QXX_INTERFACE_QSPI;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* num */
            case 6 :
            {
                /* set the num */
                if (strcmp("1", optarg) == 0)
                {
                    num = W25QXX_SECURITY_REGISTER_1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    num = W25QXX_SECURITY_REGISTER_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    num = W25QXX_SECURITY_REGISTER_3;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* status */
            case 7 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the status */
                status = hex_data & 0xFF;

                break;
            }

            /* type */
            case 8 :
            {
                /* set the type */
                if (strcmp("W25Q10", optarg) == 0)
                {
                    chip_type = W25Q10;
                }
                else if (strcmp("W25Q20", optarg) == 0)
                {
                    chip_type = W25Q20;
                }
                else if (strcmp("W25Q40", optarg) == 0)
                {
                    chip_type = W25Q40;
                }
                else if (strcmp("W25Q80", optarg) == 0)
                {
                    chip_type = W25Q80;
                }
                else if (strcmp("W25Q16", optarg) == 0)
                {
                    chip_type = W25Q16;
                }
                else if (strcmp("W25Q32", optarg) == 0)
                {
                    chip_type = W25Q32;
                }
                else if (strcmp("W25Q64", optarg) == 0)
                {
                    chip_type = W25Q64;
                }
                else if (strcmp("W25Q128", optarg) == 0)
                {
                    chip_type = W25Q128;
                }
                else if (strcmp("W25Q256", optarg) == 0)
                {
                    chip_type = W25Q256;
                }
                else if (strcmp("W25Q512", optarg) == 0)
                {
                    chip_type = W25Q512;
                }
                else if (strcmp("W25Q01", optarg) == 0)
                {
                    chip_type = W25Q01;
                }
                else if (strcmp("W25Q02", optarg) == 0)
                {
                    chip_type = W25Q02;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* wrap */
            case 9 :
            {
                /* set the wrap */
            	/*
                if (strcmp("WRAP_NONE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_NONE;
                }
                else if (strcmp("WRAP_8_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_8_BYTE;
                }
                else if (strcmp("WRAP_16_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_16_BYTE;
                }
                else if (strcmp("WRAP_32_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_32_BYTE;
                }
                else if (strcmp("WRAP_64_BYTE", optarg) == 0)
                {
                    wrap = W25QXX_BURST_WRAP_64_BYTE;
                }
                else
                {
                    return 5;
                }
                */
            	 memset(type, 0, sizeof(char) * 49);
            	 snprintf(type, 48, "stream");
            	 break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others  : question mark*/
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if(strcmp("stream", type) == 0)
    {
    	uint8_t res;
    	uint32_t current_address;
    	uint8_t uart_read_len_res;
    	uint8_t uart_result_buffer[9];

    	uint8_t merge_attempt_buffer[9];
    	uint8_t uart_read_len_sum;
    	uint8_t merge_attempt_count;

    	uint8_t NoError_Zero_Receive;

    	uint32_t hex_data;
    	uint8_t  bytesize_hex_data[4];
    	uint8_t l;
    	uint8_t *p;

    	uint32_t repeatment_required;
    	STREAM_StatusTypeDef StreamStatus;
    	StreamStatus = STREAM_IDLE;

    	if(StreamStatus == STREAM_IDLE)
    	{
    		res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
    		if (res != 0)
    		{
    			w25qxx_interface_debug_print("w25qxx: failed to init while stream.\n");
    			return 1;
    		}

    		current_address = addr;
    		repeatment_required = 320 * 240 * 2 / 4;
    		w25qxx_interface_debug_print("W25QXX_STREAM_READY\n");

    		StreamStatus = STREAM_INITED;
    	}

    	if(StreamStatus == STREAM_INITED)
    	{
    		for (uint32_t i = 0; i < repeatment_required; i++)
    		{
        		memset(merge_attempt_buffer, 0, sizeof(merge_attempt_buffer));
    			uart_read_len_sum = 0;

    			merge_attempt_count = 0;

    			p = uart_result_buffer;
    			hex_data = 0;

				memset(bytesize_hex_data, 0, sizeof(bytesize_hex_data));

    			StreamStatus =  STREAM_INUSE_NOERROR;
    			NoError_Zero_Receive = 0;

    			while(1)
    			{
    				if((StreamStatus == STREAM_INUSE_NOERROR_NOINPUT_FORLONG) || (uart_read_len_sum == 8))
    				{
    					break;
    				}
    				/*
    				 * Even after a few time since READYFORMORE yet there is no response
    				 * break out from the loop
    				 *
    				 * if the letter count is 8, break out
    				 */

    				memset(uart_result_buffer, 0, sizeof(uart_result_buffer));
    				uart_read_len_res = uart_read(uart_result_buffer, 8, &g_uart2_handle);

                    /*
                     * uart_result_buffer 는 상시 데이터를 가지고 있거나,
                     * 탈출 시 결과를 가지고 있어야 한다.
                     *
                     * 오류 처리 중간과정은 언제나 merge_attempt_buffer 에 담아둘것
                     */

    				if(StreamStatus == STREAM_INUSE_NOERROR)
    				{
    					switch(uart_read_len_res)
    					{
    						case 0:
    							NoError_Zero_Receive++;
    							if(NoError_Zero_Receive < 8)
    							{
    								w25qxx_interface_debug_print("NOERROR_ZeroInput, current try : %d\n", NoError_Zero_Receive);
    								continue;   // return to while beginning
    							}
    							else
    							{
    								StreamStatus = STREAM_INUSE_NOERROR_NOINPUT_FORLONG;
    								continue;
    							}
    							/*
    							 * READYFORMORE 신호 이후 정상상태, 누적 0개 문자, 현재 0개 문자입력
    							 * 만약 8회 미만의 현재 0개 입력에 대해서는 그저 처음으로 돌아가고
    							 * 8회 이상의 경우에 대해서는 상태를 NONOINPUT_FORLONG 으로 만들어서 탈출을 유도한다.
    							 */

    						case 8:
    						    uart_read_len_sum += uart_read_len_res;
    							break;

    							/*
    							 * READYFORMORE 신호 이후 정상상태, 누적 0개 문자, 현재 8개 문자입력
    							 * 가장 이상적인 상태로, 누적 문자의 갯수를 8로 만듦으로써 탈출을 유도한다
    							 */


    						default:
    							StreamStatus = STREAM_INUSE_ERROR_MERGEATTEMPT;
    							w25qxx_interface_debug_print("NOERROR_Lesser input, Entering Error_MergeAttempt\n");
    							uart_read_len_sum += uart_read_len_res;
    							memcpy(merge_attempt_buffer, uart_result_buffer, uart_read_len_res);
    							  // memcpy 목표지, 내용물, 복사할 길이
    							continue;

    							/*
    							 * READYFORMORE 신호 이후 정상상태, 누적 0개 문자, 현재 1~7개 문자입력
    							 *
    							 * 다음 while 반복에서부터, ERROR_MERGEATTEMPT 상태에 들어간다
    							 *
    							 * 누적 문자 증가
    							 * merge_attempt_buffer에 중간결과를 저장한다
    							 */


    					}
    				}
    				else if(StreamStatus == STREAM_INUSE_ERROR_MERGEATTEMPT)
    				{
        				if(uart_read_len_res == 0)
        				{
        					merge_attempt_count++;
        					if(merge_attempt_count >= 8)
        					{
        						memset(uart_result_buffer, '0', sizeof(uart_result_buffer) -1);
        						memcpy(&(uart_result_buffer[8-uart_read_len_sum]), merge_attempt_buffer, uart_read_len_sum);

        						uart_read_len_sum = 8;

        						w25qxx_interface_debug_print("ERROR_MERGEATTEMPT, Tried Enough Merging, Filling '0' \n");
        					}
        					else
        					{
        						w25qxx_interface_debug_print("ERROR_MERGEATTEMPT,  Currently in try : %d \n",merge_attempt_count);
        					}
        				}   /*
        		             * _ERROR_MERGEATTEMPT 상태, 누적 1~7개 문자, 현재 0개 입력
        		             *
        		             * 만약 현재 Merge 시도 횟수가 8이상이라면, merge를 포기하고 0으로 채운다
        		             *    uart_result_buffer 에는 현재 전부 아스키값 0이 차있다
        		             *    여기에 가장 마지막 자리를 제외한 위치에 전부 '0' 을 채워넣는다
        		             *
         		             *    만약 앞자리를 '0'으로 유지하고, 나머지 뒷자리를 실제 문자열로 유지하고 싶다면
        		             *    8-uart_read_len_sum 인덱스에서부터 쓰기 시작하면 된다. 쓰는 길이는 물론 uart_read_len_sum
        		             *
        		             *    uart_result_buffer 에 결과가 제작되었다.
        		             *
        		             *    sum 값을 8로 바꿔 탈출을 유도한다
        		             *
        		             *  merge 시도횟수가 8미만미아라면, 처음으로 돌아간다
        		             *
        		             *
        		             */

        				else
        				{
        					merge_attempt_count++;
        					memcpy(&(merge_attempt_buffer[uart_read_len_sum]), uart_result_buffer, uart_read_len_res);

        					uart_read_len_sum += uart_read_len_res;

        					if(uart_read_len_sum < 8)
        					{
        						w25qxx_interface_debug_print("ERROR_MERGEATTEMPT, current status : %s \n", merge_attempt_buffer);
        					}
        					else
        					{
        						w25qxx_interface_debug_print("ERROR_MERGEATTEMPT, Merge success : %s \n", merge_attempt_buffer);
        						memcpy(uart_result_buffer, merge_attempt_buffer, uart_read_len_sum);


        					}
        				}   /*   ERROR_MERGEATTEMPT 상태, 누적 1~7개 문자, 현재 7~1개 입력
        				     *
        				     *   만약 글자가 sum개(1~7) 차 있을때 다음에 쓰여야할 인덱스는 sum 위치이다.
        				     *   해당 위치 인덱스의 주소 값을 생성해서, 거기에 기록하면 이어기록하는 결과가 된다.
        				     *
        				     *   이어 기록한 후, 만약 결과가 아직 미완성이라면 다시 반복으로 돌아간다 ex.  4개 -> 6개
        				     *
        				     *   만약 결과가 총합 8개라면, uart_result_buffer 에 merge_attempt_buffer 의 값을 복사한다.
        				     *   uart_read_len_sum 값이 8 이기에, 반복에서 탈출할 수 있다
        				     *
        				     */
    				}
    				else
    				{
    					w25qxx_interface_debug_print("bad StreamStatus Value\n");
    					while(1){}  //Error!
    				}
    					/*
    					 *   불가능하긴 하지만, 만약 잘못된 StreamStatus 값으로 여기에 진입했을 시 여기에 갇힌다.
    					 */


    			}

    			if(StreamStatus == STREAM_INUSE_NOERROR_NOINPUT_FORLONG)
    			{
    				w25qxx_interface_debug_print("exit from No Input For Too long, sending NULL(black pixel) data, sending in next READYFORMORE\n");
    			}
    			   /*
    			    *    while 탈출을 NOINPUT_FORLONG 으로 탈출한경우, 0의 값 넣기를 진행할 것임을 고지한다.
    			    *    bytesize_hex_data 는 전부 0으로 초기화되어 있기에, 만약 문자열 -> hex 변환 과정을 거치지 않으면
    			    *    0의 값이 그대로 들어간다.
    			    */
    			else
    			{
    				w25qxx_interface_debug_print("exit receiving 8 letter loop : %s\n", uart_result_buffer);
					// 어떤 값이 입력됬는지 보기 위한 출력

    				if(uart_read_len_sum == 8)
    				{

    					res = hex_to_uint32(uart_result_buffer, &hex_data);
    					if(res != 0)
    					{
    						w25qxx_interface_debug_print("there was an error while translating HEX : %s\n", uart_result_buffer);
    					}
    				    /* 변환이 hex_data 로 제대로 들어왔을 때, 함수가 끝까지 수행됬을 때
    				     * res 는 0을 출력하고, 나머지의 오류 경우에대해서는 1을 출력한다.
    				     */




    					/*
    					 *  라이브러리 함수 활용으로 안쓰게 된 기능
    					l = strlen((char*)uart_result_buffer); // 일반적으로 8



    					for (uint8_t digit_index = 0; digit_index < l; digit_index++)
    					{
    						if ((p[digit_index] <= '9') && (p[digit_index] >= '0'))
    						{
    							hex_data += (p[digit_index] - '0') * (uint32_t) pow(16, l - digit_index - 1);
    						}
    						else if ((p[digit_index] <= 'F') && (p[digit_index] >= 'A'))
    						{
    							hex_data += ((p[digit_index] - 'A') + 10) * (uint32_t) pow(16, l - digit_index - 1);
    						}
    						else if ((p[digit_index] <= 'f') && (p[digit_index] >= 'a'))
    						{
    							hex_data += ((p[digit_index] - 'a') + 10) * (uint32_t) pow(16, l - digit_index - 1);
    						}
    						else
    						{
    							return 5;
    						}
    					}
    					*/

    				}
    			}
    				/*
    				 *	while 탈출 후, 정상적인 문자열이 uart_result_buffer 에 들어있는 상태
    				 *
    				 *	uart_result_buffer의 끝에는 \0 가 들어있기에, strlen 값은 상시 8이 된다.
    				 *
    				 *
    				 */


    			//전송동작

				bytesize_hex_data[0] = (uint8_t) (hex_data >> 24);
				bytesize_hex_data[1] = (uint8_t) (hex_data >> 16);
				bytesize_hex_data[2] = (uint8_t) (hex_data >> 8);
				bytesize_hex_data[3] = (uint8_t) (hex_data);


				/*
				 * 전송 되는 위치는 언제나 해당 변수의 가장 낮은 비트 8개이다.
				 *
				 *  MSB first,  << 왼쪽 shift 로 동작하는걸 믿고 작성했다.
				 *
				 */


				res = w25qxx_basic_write(current_address, (uint8_t*) bytesize_hex_data, 4);
				// 4byte를 SPI, polling 선을 통해 w25qxx 로 전송
				current_address += 4;

    			w25qxx_interface_debug_print("W25QXX_STREAM_READYFORMORE\n");
    			// calling back new data
				delay_ms(20);
				// delay for the terminal program to become Idle state


    		}
    		w25qxx_interface_debug_print("Stream Sending (320 * 240 * 2 / 4) * 4 bytes were send \n");
    		StreamStatus = STREAM_IDLE;
    		res = w25qxx_basic_deinit();
    		if (res != 0)
    		{
    			w25qxx_interface_debug_print("w25qxx: failed to deinit while stream.\n");
    			return 1;
    		}

    	}
    	return 0;
    }
    else if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* run reg test */
        res = w25qxx_register_test(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* run read test */
        res = w25qxx_read_test(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_power-down", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* power down */
        res = w25qxx_basic_power_down();
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: power down successful.\n");
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* wake up */
        res = w25qxx_basic_wake_up();
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: wake up successful.\n");
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_chip-erase", type) == 0)
    {
        uint8_t res;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* output */
        w25qxx_interface_debug_print("w25qxx: start chip erase.\n");

        /* chip erase */
        res = w25qxx_basic_chip_erase();
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_get-id", type) == 0)
    {
        uint8_t res;
        uint8_t manufacturer;
        uint8_t device_id;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* get id */
        res = w25qxx_basic_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n",
                                         manufacturer, device_id);
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
    	/*
    	 *  addr 위치의 값을 1byte 읽어옴
    	 */
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* read */
        res = w25qxx_basic_read(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %6x is %2x.\n", addr, dat);
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("e_write", type) == 0)
    {

    	/*
    	 *  addr 위치에 data 중 하위 8비트를 씀
    	 */
        uint8_t res;
        uint8_t dat;

        /* check the interface */
        if (interface != W25QXX_INTERFACE_SPI)
        {
            return 5;
        }

        /* basic init */
        res = w25qxx_basic_init(chip_type, interface, W25QXX_BOOL_FALSE);
        if (res != 0)
        {
            return 1;
        }

        /* write */
        dat = data & 0xFF;
        res = w25qxx_basic_write(addr, (uint8_t *)&dat, 1);
        if (res != 0)
        {
            (void)w25qxx_basic_deinit();

            return 1;
        }
        else
        {
            w25qxx_interface_debug_print("w25qxx: addr %d write data %d.\n", addr, dat);
        }

        /* basic deinit */
        (void)w25qxx_basic_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {

        help:
		/*
        w25qxx_interface_debug_print("Usage:\n");
        w25qxx_interface_debug_print("  w25qxx (-i | --information)\n");
        w25qxx_interface_debug_print("  w25qxx (-h | --help)\n");
        w25qxx_interface_debug_print("  w25qxx (-p | --port)\n");
        w25qxx_interface_debug_print("  w25qxx (-t reg | --test=reg) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-t read | --test=read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e power-down | --example=power-down) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e wake-up | --example=wake-up) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e chip-erase | --example=chip-erase) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e get-id | --example=get-id) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e read | --example=read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e write | --example=write) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-power-down | --example=advance-power-down) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-wake-up | --example=advance-wake-up) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-chip-erase | --example=advance-chip-erase) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-id | --example=advance-get-id) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-read | --example=advance-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-write | --example=advance-write) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-page-program | --example=advance-page-program) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-erase-4k | --example=advance-erase-4k) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-erase-32k | --example=advance-erase-32k) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-erase-64k | --example=advance-erase-64k) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-fast-read | --example=advance-fast-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-status1 | --example=advance-get-status1) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-status2 | --example=advance-get-status2) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-status3 | --example=advance-get-status3) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-set-status1 | --example=advance-set-status1) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --status=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-set-status2 | --example=advance-set-status2) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --status=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-set-status3 | --example=advance-set-status3) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --status=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-get-jedec-id | --example=advance-get-jedec-id) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-global-lock | --example=advance-global-lock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-global-unlock | --example=advance-global-unlock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-block-lock | --example=advance-block-lock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-block-unlock | --example=advance-block-unlock) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-read-block | --example=advance-read-block) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-reset | --example=advance-reset) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] [--interface=<spi | qspi>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-read | --example=advance-spi-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>] --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-dual-output-read | --example=advance-spi-dual-output-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-quad-output-read | --example=advance-spi-quad-output-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-dual-io-read | --example=advance-spi-dual-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-quad-io-read | --example=advance-spi-quad-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-word-quad-io-read | --example=advance-spi-word-quad-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-octal-word-quad-io-read | --example=advance-spi-octal-word-quad-io-read) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-page-program-quad-input | --example=advance-spi-page-program-quad-input) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --addr=<address> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-get-id-dual-io | --example=advance-spi-get-id-dual-io) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-get-id-quad-io | --example=advance-spi-get-id-quad-io) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-get-sfdp | --example=advance-spi-get-sfdp) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-write-security | --example=advance-spi-write-security) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --num=<1 | 2 | 3> --data=<hex>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-read-security | --example=advance-spi-read-security) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --num=<1 | 2 | 3>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-qspi-set-read-parameters | --example=advance-qspi-set-read-parameters) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --dummy=<DUMMY_2_33MHZ | DUMMY_4_55MHZ | DUMMY_6_80MHZ | DUMMY_8_80MHZ> --length=<8 | 16 | 32 | 64>\n");
        w25qxx_interface_debug_print("  w25qxx (-e advance-spi-set-burst | --example=advance-spi-set-burst) [--type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>]\n");
        w25qxx_interface_debug_print("         --wrap=<WRAP_NONE | WRAP_8_BYTE | WRAP_16_BYTE | WRAP_32_BYTE | WRAP_64_BYTE>\n");
        w25qxx_interface_debug_print("\n");
        w25qxx_interface_debug_print("Options:\n");
        w25qxx_interface_debug_print("      --addr=<address>               Set the operator address and it is hexadecimal.([default: 0x00000000])\n");
        w25qxx_interface_debug_print("      --data=<hex>                   Set the input data and it is hexadecimal.([default: 0x00000000])\n");
        w25qxx_interface_debug_print("      --dummy=<DUMMY_2_33MHZ | DUMMY_4_55MHZ | DUMMY_6_80MHZ | DUMMY_8_80MHZ>\n");
        w25qxx_interface_debug_print("                                     Set the dummy.([default: DUMMY_8_80MHZ])\n");
        w25qxx_interface_debug_print("  -e <power-down | wake-up | chip-erase | get-id | read | write | advance-power-down | advance-wake-up | advance-chip-erase\n");
        w25qxx_interface_debug_print("     | advance-get-id | advance-read | advance-write | advance-page-program | advance-erase-4k | advance-erase-32k | advance-erase-64k\n");
        w25qxx_interface_debug_print("     | advance-fast-read | advance-get-status1 | advance-get-status2 | advance-get-status3 | advance-set-status1 | advance-set-status2\n");
        w25qxx_interface_debug_print("     | advance-set-status3 | advance-get-jedec-id | advance-global-lock | advance-global-unlock | advance-block-lock | advance-block-unlock\n");
        w25qxx_interface_debug_print("     | advance-read-block | advance-reset | advance-spi-read | advance-spi-dual-output-read | advance-spi-quad-output-read\n");
        w25qxx_interface_debug_print("     | advance-spi-dual-io-read | advance-spi-quad-io-read | advance-spi-word-quad-io-read | advance-spi-octal-word-quad-io-read\n");
        w25qxx_interface_debug_print("     | advance-spi-page-program-quad-input | advance-spi-get-id-dual-io | advance-spi-get-id-quad-io | advance-spi-get-sfdp\n");
        w25qxx_interface_debug_print("     | advance-spi-write-security | advance-spi-read-security | advance-qspi-set-read-parameters | advance-spi-set-burst>, --example=<power-down\n");
        w25qxx_interface_debug_print("     | wake-up | chip-erase | get-id | read | write | advance-power-down | advance-wake-up | advance-chip-erase\n");
        w25qxx_interface_debug_print("     | advance-get-id | advance-read | advance-write | advance-page-program | advance-erase-4k | advance-erase-32k | advance-erase-64k\n");
        w25qxx_interface_debug_print("     | advance-fast-read | advance-get-status1 | advance-get-status2 | advance-get-status3 | advance-set-status1 | advance-set-status2\n");
        w25qxx_interface_debug_print("     | advance-set-status3 | advance-get-jedec-id | advance-global-lock | advance-global-unlock | advance-block-lock | advance-block-unlock\n");
        w25qxx_interface_debug_print("     | advance-read-block | advance-reset | advance-spi-read | advance-spi-dual-output-read | advance-spi-quad-output-read\n");
        w25qxx_interface_debug_print("     | advance-spi-dual-io-read | advance-spi-quad-io-read | advance-spi-word-quad-io-read | advance-spi-octal-word-quad-io-read\n");
        w25qxx_interface_debug_print("     | advance-spi-page-program-quad-input | advance-spi-get-id-dual-io | advance-spi-get-id-quad-io | advance-spi-get-sfdp\n");
        w25qxx_interface_debug_print("     | advance-spi-write-security | advance-spi-read-security | advance-qspi-set-read-parameters | advance-spi-set-burst>\n");
        w25qxx_interface_debug_print("                                     Run the driver example.\n");
        w25qxx_interface_debug_print("  -h, --help                         Show the help.\n");
        w25qxx_interface_debug_print("      --length=<8 | 16 | 32 | 64>    Set the dummy length.([default: 8])\n");
        w25qxx_interface_debug_print("  -i, --information                  Show the chip information.\n");
        w25qxx_interface_debug_print("      --interface=<spi | qspi>       Set the chip interface.([default: spi])\n");
        w25qxx_interface_debug_print("      --num=<1 | 2 | 3>              Set the security number.([default: 1])\n");
        w25qxx_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        w25qxx_interface_debug_print("      --status=<hex>                 Set the status and it is hexadecimal.([default: 0x00])\n");
        w25qxx_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        w25qxx_interface_debug_print("                                     Run the driver test.\n");
        w25qxx_interface_debug_print("      --type=<W25Q80 | W25Q16 | W25Q32 | W25Q64 | W25Q128 | W25Q256>\n");
        w25qxx_interface_debug_print("                                     Set the chip type.([default: W25Q128])\n");
        w25qxx_interface_debug_print("      --wrap=<WRAP_NONE | WRAP_8_BYTE | WRAP_16_BYTE | WRAP_32_BYTE | WRAP_64_BYTE>\n");
        w25qxx_interface_debug_print("                                     Set the wrap.([default: WRAP_8_BYTE])\n");
    	 */

    	 w25qxx_interface_debug_print("help is not supported due to flash region size wise");
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        w25qxx_info_t info;

        /* print w25qxx info */
        w25qxx_info(&info);
        w25qxx_interface_debug_print("w25qxx: chip is %s.\n", info.chip_name);
        w25qxx_interface_debug_print("w25qxx: manufacturer is %s.\n", info.manufacturer_name);
        w25qxx_interface_debug_print("w25qxx: interface is %s.\n", info.interface);
        w25qxx_interface_debug_print("w25qxx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        w25qxx_interface_debug_print("w25qxx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        w25qxx_interface_debug_print("w25qxx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        w25qxx_interface_debug_print("w25qxx: max current is %0.2fmA.\n", info.max_current_ma);
        w25qxx_interface_debug_print("w25qxx: max temperature is %0.1fC.\n", info.temperature_max);
        w25qxx_interface_debug_print("w25qxx: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        w25qxx_interface_debug_print("w25qxx: SPI interface SCK connected to GPIOA PIN5.\n");
        w25qxx_interface_debug_print("w25qxx: SPI interface MISO connected to GPIOA PIN6.\n");
        w25qxx_interface_debug_print("w25qxx: SPI interface MOSI connected to GPIOA PIN7.\n");
        w25qxx_interface_debug_print("w25qxx: SPI interface CS connected to GPIOA PIN4.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}


uint8_t W25QXX_WITH_ILI9341_Draw_Image(uint32_t addr, uint8_t orientation)
{

	uint32_t current_address;
	uint8_t  res;
	uint32_t repeatment_required;

	uint8_t receive_result[2048];


	res = w25qxx_basic_init(W25Q64, W25QXX_INTERFACE_SPI,  W25QXX_BOOL_FALSE);
	if (res != 0)
	{
		uart2_print("there was an error while Initing w25qxx - snow tiger\n");
		return 1;
	}

	//ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_VERTICAL_2);
	ILI9341_Set_Address(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);

	current_address = addr;
	repeatment_required = 320 * 240 * 2 / 2048;

	for(uint32_t i=0; i < repeatment_required; i++)
	{

		memset(receive_result, 0, sizeof(receive_result));
		// uint32_t debug_tickstart = HAL_GetTick();
		/*
		 *   각 단계에 얼마만큼 시간이 걸리는지 측정하는 debug_tickstart 변수
		 */

		res = w25qxx_FastRead (current_address, (uint8_t *)receive_result, 2048);

		if (res != 0)
		{
			(void)w25qxx_basic_deinit();
			// 오류 상태
			return 2;
		}

		// uint32_t debug_tickelapsed = HAL_GetTick() - debug_tickstart;

		//uart2_print("Elapsed time between w25qxx_basic_read  :  %d ms\n", debug_tickelapsed);


		/*
		 * w25qxx의 CS 핀은 basic_read ->w25qxx_interface_spi_qspi_write_read -> spi_write_read  를 통해
		 * 잘 관리되고 있으므로 (사용 후 CS핀 High), W25qxx의 CS 핀 문제는 신경 쓸 필요없다.
		 */

		// debug_tickstart = HAL_GetTick();

		ILI9341_Sending_Pixels(receive_result, 2048);

		// debug_tickelapsed = HAL_GetTick() - debug_tickstart;

		//uart2_print("Elapsed time betweenILI9341_Sending_Pixels : %d ms\n", debug_tickelapsed);

		current_address +=2048;
	}

	 (void)w25qxx_basic_deinit();

	 return 0;

}

/*
 *  입력된 8개의 HEX값 문자열을 기반으로 그것을 32bit 의 정수를 제작함
 *  입력은 str 위치에,  출력은 result 에 받아준 주소에 값을 쓴다.
 *
 *  함수의 출력값은 함수의 결과를 표시하기 위함
 */


uint8_t hex_to_uint32(const char *str, uint32_t *result)
{
    char *endptr;
    unsigned long value;

    if (str == NULL ||  *str == '\0'|| result == NULL)
        return 1;
    // 입력이 존재하고, 출력을 받을곳이 존재하는지 확인

    errno = 0;
    value = strtoul(str, &endptr, 16);
    // value 는 최대 4byte  8개의 hex 자리이므로 1줄의 입력을 전부 그대로 가져와도 좋을듯 하다.

    // 검사 : 문자열이 시작부터 \0  이상한 문자가 문자열 내에 존재   범위 초과
    if (endptr == str ||  *endptr != '\0' || errno == ERANGE)
    {
        return 1;
    }

    *result = (uint16_t)value;
    return 0;
}



uint8_t  w25qxx_FastRead (uint32_t addr, uint8_t *receive_buffer, uint32_t receive_ammount)
{
   /*
    * addr 에는 가장 낮은 자리에 addr 값이 들어있다
    * 0x100000 을 예시로, 하위 3byte에 주소 값이 들어있다
    *
    * w25qxx의 주소 정보는 3byte 모드로,  최상위 byte는 안 쓰인다 봐도 된다
    *
    */

	uint8_t transmit_buffer[5];
	uint8_t res;


	/* 시간소모 측정 디버그용 변수 */
	uint32_t debug_tickelapsed;
	uint32_t debug_tickstart;

	debug_tickstart = HAL_GetTick();

	/* FAST_READ 명령과 주소를 담을 버퍼 제작*/

	transmit_buffer[0] = 0x0B;      /*W25QXX_COMMAND_FAST_READ 의 매크로값*/
	transmit_buffer[1] = (addr >> 16) & 0xFF;
	transmit_buffer[2] = (addr >> 8) & 0xFF;
	transmit_buffer[3] = (addr >> 0) & 0xFF;
	transmit_buffer[4] = 0x00;   /* dummy */

	debug_tickelapsed = HAL_GetTick() - debug_tickstart;
	/* 버퍼를 제작하는데 걸리는 시간 측정 */
	uart2_print("Elapsed time between buffer creation:  %d ms\n", debug_tickelapsed);

	/* set cs low    CS 핀 설정,  사용을 마치고 꼭 High 로 돌아와야함 */
	HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

	debug_tickstart = HAL_GetTick();

	/* 명령과 주소를 전송 */
	res = HAL_SPI_Transmit(&hspi1, transmit_buffer, 5, 100);
	if (res != HAL_OK)
	{
		HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);
		return 1;
	}

	debug_tickelapsed = HAL_GetTick() - debug_tickstart;
	uart2_print("Elapsed time between HAL_SPI_Transmit  : %d ms\n", debug_tickelapsed);


	/*
	 * 만약 SPI 통신의 기본 byte 단위를 2byte 로 변경하고자 하는 경우,
	 * HAL_SPI_Transmit (... , ... , 3, ... ) 으로해야하고
	 *
	 * transmit_buffer는 6byte 크기여야하며  4,5 번 인덱스는 더미가 채워져 있어야한다
	 *
	 * 아니면 2byte 모드에서 마지막 짜투리 전송으로 1byte 만 전송하는 방법이 있을까?
	 * 5byte 받기로 약속한 w25qxx 에도 6byte 보내면 좋지 않아질거같은데
	 */

	debug_tickstart = HAL_GetTick();

	/* 2048byte 를 수신받기 시작 */

	res = HAL_SPI_Receive(&hspi1, receive_buffer, receive_ammount, 100);
	if (res != HAL_OK)
	{
		HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);
		return 1;
	}

	debug_tickelapsed = HAL_GetTick() - debug_tickstart;
	uart2_print("Elapsed time between HAL_SPI_Receive:  %d ms\n", debug_tickelapsed);

	/*CS 핀 High 로 돌아오게 만듬 */
	HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

	return 0;
}




