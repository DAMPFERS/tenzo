import hid

VENDOR_ID = 0x045E  # Microsoft (Xbox)
PRODUCT_ID = 0x028E

try:
    gamepad = hid.device()
    gamepad.open(VENDOR_ID, PRODUCT_ID)
    gamepad.set_nonblocking(True)  # Устанавливаем неблокирующий режим
    print("Геймпад подключен!")

    while True:
        data = gamepad.read(64)
        if data:
            print(f"Получены данные: {data}")
except Exception as e:
    print(f"Ошибка: {e}")
finally:
    gamepad.close()


# import pygame

# # Инициализация pygame
# pygame.init()

# # Подключение геймпада
# if pygame.joystick.get_count() == 0:
#     print("Геймпад не обнаружен!")
# else:
#     joystick = pygame.joystick.Joystick(0)
#     joystick.init()
#     print(f"Подключен геймпад: {joystick.get_name()}")

# # Чтение данных
# try:
#     while True:
#         pygame.event.pump()
#         x = joystick.get_axis(0)  # Перемещение по оси X левого стика
#         y = joystick.get_axis(1)  # Перемещение по оси Y левого стика
#         print(f"Стик X: {x}, Стик Y: {y}")
# except KeyboardInterrupt:
#     print("Программа завершена.")
# finally:
#     pygame.quit()


# import hid

# for device in hid.enumerate():
#     print(device)
