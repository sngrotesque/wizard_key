def build_next(patt):
    # 这个函数用于构建next数组，next数组记录了匹配串中每个位置的最长公共前后缀的长度
    next = [0 for _ in range(len(patt))] # 初始化next数组，长度等于子串的长度，每个元素为0，表示没有公共前后缀
    prefix_len = 0 # 初始化最长公共前后缀的长度为0
    i = 1 # 初始化当前位置为1
    while i < len(patt): # 遍历匹配串的每个位置
        if patt[prefix_len] == patt[i]: # 如果当前位置和最长公共前后缀的下一个字符相同
            prefix_len += 1 # 最长公共前后缀的长度加1
            next[i] = prefix_len # 将最长公共前后缀的长度更新到next数组中对应位置
            i += 1 # 当前位置加1
        else: # 如果当前位置和最长公共前后缀的下一个字符不同
            if prefix_len == 0: # 如果最长公共前后缀的长度为0，表示没有公共前后缀
                next[i] = 0 # 将0更新到next数组中对应位置
                i += 1 # 当前位置加1
            else: # 如果最长公共前后缀的长度不为0，表示有公共前后缀
                prefix_len = next[prefix_len - 1] # 根据next数组中的信息，更新最长公共前后缀的长度为上一个位置的值
    return next # 返回next数组

def kmp_match(string, patt):
    # 这个函数用于在主串中查找匹配串，如果找到，返回匹配串在主串中的起始位置；如果没有找到，返回-1
    next = build_next(patt) # 调用build_next函数，构建next数组

    i = 0 # 初始化主串的当前位置为0
    j = 0 # 初始化匹配串的当前位置为0

    while i < len(string): # 遍历主串的每个位置
        if string[i] == patt[j]: # 如果主串和匹配串的当前位置字符相同
            i += 1 # 主串的当前位置加1
            j += 1 # 匹配串的当前位置加1
        elif j > 0: # 如果主串和匹配串的当前位置字符不同，并且匹配串的当前位置不为0，表示有部分匹配成功
            j = next[j - 1] # 根据next数组中的信息，更新匹配串的当前位置为上一个最长公共前后缀的长度
        else: # 如果主串和匹配串的当前位置字符不同，并且匹配串的当前位置为0，表示没有匹配成功
            i += 1 # 主串的当前位置加1

        if j == len(patt): # 如果匹配串的当前位置等于匹配串的长度，表示完全匹配成功
            return i - j # 返回匹配串在主串中的起始位置，即主串的当前位置减去匹配串的长度
    return -1 # 如果遍历完主串都没有找到匹配，返回-1

string = '.............anc...12.3..ac...123'
sub = 'nc'

print(kmp_match(string, sub))

