from PIL import Image
import os
import tkinter as tk
from tkinter import filedialog, messagebox


# ============================================================
# 설정
# ============================================================

WIDTH = 320
HEIGHT = 240

READY_MESSAGE = "W25QXX_STREAM_READY"


# ============================================================
# RGB888 -> RGB565 변환
# 상위비트가 큰 값을 의미하고, 아래 비트 값은 디테일을 의미한다.
# 따라서 비트 8개를 5개로 줄여야한다면, 상위비트를 살려야한다
# ============================================================

def rgb888_to_rgb565(r, g, b):
    """
    24-bit RGB888을 16-bit RGB565로 변환

    R : 5 bit
    G : 6 bit
    B : 5 bit
    """

    r5 = (r >> 3) & 0x1F
    g6 = (g >> 2) & 0x3F
    b5 = (b >> 3) & 0x1F

    rgb565 = (r5 << 11) | (g6 << 5) | b5

    return rgb565


# ============================================================
# PNG -> Tera Term Macro
# ============================================================

def convert_png_to_ttl(input_path, output_path):

    # --------------------------------------------------------
    # 이미지 열기
    # --------------------------------------------------------

    try:
        image = Image.open(input_path)
        image = image.convert("RGB")
    except Exception as e:
        raise Exception(f"PNG 파일을 열 수 없습니다.\n\n{e}")


    # --------------------------------------------------------
    # 해상도 검사
    # --------------------------------------------------------

    if image.width != WIDTH or image.height != HEIGHT:
        raise Exception(
            f"이미지 해상도가 올바르지 않습니다.\n\n"
            f"현재 해상도 : {image.width} x {image.height}\n"
            f"필요한 해상도 : {WIDTH} x {HEIGHT}"
        )


    # --------------------------------------------------------
    # RGB565 데이터 생성
    #
    # 세로열 우선
    #
    # (0,0)
    # (0,1)
    # ...
    # (0,239)
    # (1,0)
    # (1,1)
    # ...
    #
    # 여기서 x = 열, y = 행
    # --------------------------------------------------------

    hex_pixels = []

    for x in range(WIDTH):

        for y in range(HEIGHT):

            r, g, b = image.getpixel((x, y))

            rgb565 = rgb888_to_rgb565(r, g, b)

            # 16-bit = 4자리 HEX
            hex_value = f"{rgb565:04X}"

            hex_pixels.append(hex_value)


    # --------------------------------------------------------
    # 픽셀 2개씩 합치기
    #
    # 예:
    #
    # 1234
    # 5678
    #
    # ->
    #
    # 12345678
    # --------------------------------------------------------

    if len(hex_pixels) % 2 != 0:
        raise Exception("픽셀 개수가 홀수입니다.")

    combined_data = []

    for i in range(0, len(hex_pixels), 2):

        combined = hex_pixels[i] + hex_pixels[i + 1]

        combined_data.append(combined)


    # --------------------------------------------------------
    # Tera Term Macro 생성
    #
    # wait -> sendln -> wait -> sendln ...
    # --------------------------------------------------------

    with open(output_path, "w", encoding="utf-8", newline="\r\n") as f:

        for data in combined_data:

            f.write(f"wait '{READY_MESSAGE}'\n")
            f.write(f"sendln '{data}'\n")


    return len(hex_pixels), len(combined_data)


# ============================================================
# 파일 선택
# ============================================================

def select_file():

    root = tk.Tk()
    root.withdraw()

    input_path = filedialog.askopenfilename(
        title="320x240 PNG 선택",
        filetypes=[
            ("PNG files", "*.png"),
            ("All files", "*.*")
        ]
    )

    if not input_path:
        return

    # --------------------------------------------------------
    # 출력 파일 이름
    # --------------------------------------------------------

    base_name = os.path.splitext(input_path)[0]

    output_path = base_name + "_RGB565.ttl"


    # --------------------------------------------------------
    # 변환
    # --------------------------------------------------------

    try:

        pixel_count, line_count = convert_png_to_ttl(
            input_path,
            output_path
        )

        messagebox.showinfo(
            "변환 완료",
            f"변환이 완료되었습니다.\n\n"
            f"입력 : {os.path.basename(input_path)}\n"
            f"출력 : {os.path.basename(output_path)}\n\n"
            f"픽셀 수 : {pixel_count:,}\n"
            f"전송 라인 : {line_count:,}"
        )

    except Exception as e:

        messagebox.showerror(
            "변환 실패",
            str(e)
        )


# ============================================================
# 실행
# ============================================================

if __name__ == "__main__":
    select_file()