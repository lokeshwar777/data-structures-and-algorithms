'''
returns index at which the element first occurs if it exists else -1
'''


def linear_search(nums, search_element):
    n = len(nums)
    for index in range(n):
        if nums[index] == search_element:
            return index
    return -1


'''
Performs linear search:
- returns index at which the element first occurs if it exists else -1
- TC -> O(n), SC -> O(1)
'''


def pythonic_linear_search(nums, search_element):
    for index, num in enumerate(nums):
        if num == search_element:
            return index
    return -1


'''
In-built methods
- find() -> gives you ValueError if not found
'''
