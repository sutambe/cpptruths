/**
 * @param {number[]} nums
 * @param {number} target
 * @return {number[]}
 */
var twoSum = function(nums, target) {
    var nums_map = {};
    for (var i = 0;i < nums.length; i++) {
        var number = nums[i];
        nums_map[number] = i;
    }
    for (var i = 0;i < nums.length; i++) {
        var number = nums[i];
        var diff = target - number;
        var index = nums_map[diff];
        if (index >= 0 && index != i) {
            return [i, index];
        }
    }
};
