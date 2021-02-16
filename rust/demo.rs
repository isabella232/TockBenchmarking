// This example just prints "Hello Tock World" to the terminal.
// Run `tockloader listen`, or use any serial program of your choice
//  (e.g. `screen`, `minicom`) to view the message.

#![no_std]
#![feature(asm)]

use libtock::println;
use libtock::result::TockResult;

libtock_core::stack_size! {0x400}

#[libtock::main]
async fn main() -> TockResult<()> {
    let drivers = libtock::retrieve_drivers()?;

    drivers.console.create_console();

    let start: u32;
    let end: u32;

    unsafe{
      asm!("RDCYCLE {}", out(reg) start);
    }

    for i in 1..10{
      println!("Hello World {}", i);
    }

    unsafe{
      asm!("RDCYCLE {}", out(reg) end);
    }

    println!("Hello Demo Took {} cycles", end - start);

    Ok(())
}
