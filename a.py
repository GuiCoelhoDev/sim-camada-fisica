code = [0,1,0,0,1,1,0,0,0,1,1]
res = [1]


def invert(val):
    if val == 0:
        return 1
    else:
        return 0


print(res)
for val in code:
    nextVal = -1
    lastValue = res[-1]

    if val == 0:
        nextVal = invert(lastValue)
    if val == 1:
        nextVal = lastValue

    res.append(nextVal)
    res.append(invert(nextVal))
    
res.pop(0)
print(res)
