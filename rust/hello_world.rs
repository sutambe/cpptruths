use std::rc::Rc;
use std::sync::{Arc, Mutex};

fn main() {
 let a = 10;
 if a == 10 {
    println!("Hello World!");
 }
 let b = Box::new(20);
 let c = Rc::new(Box::new(30));
 let d = Arc::new(Mutex::new(40));
 println!("a: {:?}, b: {:?}, c: {:?}, d: {:?}", a, b, c, d);
}
