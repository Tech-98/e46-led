def anagram(str1, str2):
    if len(str1) != len(str2) or len(str1) == 0 or len(str2) == 0:
        return False
    hashmap = {}
    for i in str1:
        if i in hashmap:
            hashmap[i] += 1
        else:
            hashmap[i] = 1

    for i in str2:
        if i in hashmap:
            hashmap[i] -= 1
        else:
            return False

    for i in hashmap:

        if hashmap[i] != 0:
            return False
    return True

print(anagram('', ''))

