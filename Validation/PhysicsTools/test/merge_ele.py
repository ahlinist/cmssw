file1 = open('test/index.html')
file2 = open('ObjectTemplateFiles/index_ele.html')
file3 = open('test/merged.html', 'w')

for line in file1:
    if line.find('INSERT HERE') != -1:
        for line in file2:
            file3.write(line)
    else:
        file3.write(line)
