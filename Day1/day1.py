with open('input.txt') as f:
    contents = f.read()
contents = contents.split('\n')

Sum = 0
biggest = [0, 0, 0]
for line in contents:
    if line == '':
        for i in range(len(biggest)):
            if Sum > biggest[i]:
                temp = biggest[i]
                biggest[i] = Sum
                Sum = temp
                i = 0
        Sum = 0
    else:
        Sum += int(line)

total = sum(biggest)
print(biggest)
print(total)