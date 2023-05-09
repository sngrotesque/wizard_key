minValue = 1
maxValue = 8192

result = []

for index in range(minValue, maxValue + 1):
    for x in range(2, index):
        # print(index, x, index % x)
        if (index % x == 0):
            break
    else:
        result.append(index)

print(result)
print(len(result))
