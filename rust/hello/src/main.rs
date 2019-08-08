use std::ops::{Add};
use regex::Regex;
use std::fs::File;
use std::io::BufReader;
use std::io::prelude::*;

extern crate regex;

fn greet_world() {
  println!("Hello, world!"); // our old friend.

  let southern_germany = "";
  let japan = "";

  let regions = [southern_germany, japan];

  for region in regions.iter() {
    println!("{}", &region);
  }
}

fn add_with_lifetimes<'a, 'b>(i: &'a i32, j: &'b i32) -> i32 {
  *i + *j
}

fn add_generic<T: Add<Output = T>>(i: T, j: T) -> T {
  i + j
}

fn regex() {
  let re = Regex::new("picture").unwrap();

  let quote = "Every face, every shop, bedroom window, public-house, and
dark square is a picture feverishly turned--in search of what?
It is the same with books. What do we seek through millions of pages?";

 for line in quote.lines() {
   match re.find(line) {
     Some(_) => println!("{}", line),
     None => (),
   }
 }
}

fn file_io() {
  let f = File::open("Cargo.toml").unwrap();
  let mut reader = BufReader::new(f);
  let mut line = String::new();
  loop {
    let len = reader.read_line(&mut line).unwrap();
    if len == 0 {
      break
    }
    println!("{} ({} bytes long)", line, len);
    line.truncate(0);
  }
}

fn pointers() {
  let a: i64 = 42;
  let a_ptr:*mut i64 = &a as *const i64 as *mut i64;
  unsafe {
    *a_ptr = 43;
  }
  println!("a: {} ({:p})", a, a_ptr);
}

fn is_strong(pw: String) -> bool {
  pw.len() > 5
}

fn is_strong_generic<T: AsRef<str>>(pw: T) -> bool {
  pw.as_ref().len() > 5
}

// Implicit conversion to string inside a function looks expensive
fn is_strong_conversion<T: Into<String>>(pw: T) -> bool {
  pw.into().len() > 5
}

fn alloc_dealloc() {
  let a = Box::new(10);
  let b = Box::new(20);
  //std::mem::drop(a);
  let result = *a + *b ;
}

fn noop1() {}
fn noop2() {}
fn function_pointer() {
  let fn_ptr = noop1 as usize;
  let fn_ptr = noop2 as *const fn() -> () as usize;
  println!("noop as usize: 0x{:x}", fn_ptr);
}

fn main() {
  let pw = "justok";
  // is_strong(pw); // fails to compile. Use conversion method pw.to_string()
  is_strong_generic(pw);
  is_strong_conversion(pw);
  pointers();
  file_io();
  regex();
  greet_world();
  function_pointer();
  let res = add_with_lifetimes(&10, &20);
  let res2 = add_generic(100, 200);
  println!("{}", res);
  println!("{}", res2);
  const JMP_BUF_WIDTH: usize = std::mem::size_of::<usize>() * 8;
  println!("size of JMP_BUF_WIDTH = {:?}", JMP_BUF_WIDTH);
}
