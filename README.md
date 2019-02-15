# -pacman-vs-ghost-game-


# Rule : luật chơi
- Pacman, Ghost chỉ được di chuyển mỗi lần 1 ô lên trên (y-1), xuống dưới (y+1), sang trái (x-1), sang phải (x+1)
- Pacman, Ghost chỉ đi vào các ô có dấu cách ‘ ’, hoặc ô có dot ‘.’ , KHÔNG đi vào tường ‘#’
- NẾU Pacman đi vào ô dot ‘.’ thì Pacman sẽ ăn được dot và thay ‘.’ bằng dấu cách ‘ ’. Nếu Pacman ăn hết dot trong 2*m*n bước thì Pacman thắng
- NẾU Ghost đi vào ô Pacman thì Ghost sẽ ăn được Pacman
- Khi vào game Pacman sẽ đứng ở vị trí gần góc trên bên trái nhất và Ghost sẽ đứng ở vị trí gần góc dưới bên phải nhất
- File pacman.cpp điều khiển pacman, file ghost.cpp điều khiển ghost

File input: "C:\\pacman.txt" lưu thông tin mê cung lúc đầu, có cấu trúc:
+ dòng đầu ghi kích thước của mê cung n m
+ n dòng tiếp theo ghi 1 trong 4 ký tự (‘.’: dot, ‘#’: tường, và dấu cách ‘ ‘: đường đi). Mỗi dòng có m kí tự.
