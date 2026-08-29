import re

INPUT_FILE = "snow_tiger.h"
OUTPUT_FILE = "snow_tiger_ttl_ver.txt"


with open(INPUT_FILE, "r", encoding="utf-8") as f:
    text = f.read()

# 0xXX 형태의 데이터만 추출
bytes_list = re.findall(r'0x([0-9a-fA-F]{2})', text)

# 4바이트씩 묶어서 출력
with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    for i in range(0, len(bytes_list), 4):
        chunk = bytes_list[i:i+4]

        # 마지막에 4바이트가 안 남는 경우
        if len(chunk) < 4:
            break

        # 0x 제거 후 4바이트 = 8자리 hex
        data = ''.join(chunk).lower()

        f.write(data + "\n")
        f.write("wait 'W25QXX_STREAM_READYFORMORE'\n")

print(f"완료: {OUTPUT_FILE}")