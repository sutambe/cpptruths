/**
 * @param {string} s
 * @return {boolean}
 */
var isValid = function(s) {
    var stack = [];
    for(var c of s) {
        if (c === '[' || c === '{' || c === '(') {
            stack.push(c);
        } 
        else if (c === ']' && stack[stack.length-1] === '[') {
            stack.pop();
        }
        else if (c === '}' && stack[stack.length-1] === '{') {
            stack.pop();
        }
        else if (c === ')' && stack[stack.length-1] === '(') {
            stack.pop();
        }
        else {
            return false;
        }
    }
    if (stack.length == 0) {
        return true;
    }
    return false;
};
