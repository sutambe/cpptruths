//var variadic = require('allong.es').variadic;
//var trampoline = function(fn) {
//  return variadic(function(ars){
//    var result = fn.apply(this, args);
//    while(result instanceof Function) {
//      result = result();
//    }
//    return result;
//  });
//};

var trampoline = function(fn) {
  return function(arg1, arg2) {
    var result = fn(arg1, arg2);
    while(result instanceof Function) {
      result = result();
    }
    return result;
  };
};

function add(n) {
  var _add = trampoline(function myself(acc, n) {
    return n? function() { return myself(acc+n, n-1); } : acc; 
  });
 return _add(0, n);  
}

function add_r(acc, n) {
  return n? n+add_r(acc, n-1) : 0;
}

console.log(add(5));
