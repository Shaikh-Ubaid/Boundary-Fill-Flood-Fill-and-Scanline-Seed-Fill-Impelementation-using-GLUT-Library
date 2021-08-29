# // int h = glutGet( GLUT_WINDOW_HEIGHT );
# // int w = glutGet( GLUT_WINDOW_WIDTH );

scaleX = 2
scaleY = 2
centerX = 120
centerY = 80

moveFactor = 250

n = int(input())
for _ in range(n):
    x, y = map(int, input().split())
    # translatedX = x - centerX
    # translatedY = y - centerY

    # scaledX = translatedX * scaleX
    # scaledY = translatedY * scaleY

    # x = scaledX + centerX
    # y = scaledY + centerY

    x += moveFactor
    y += moveFactor

    print(x, y)