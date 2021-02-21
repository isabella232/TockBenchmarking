#![no_std]
#![feature(asm)]

use libtock::println;
use libtock::result::TockResult;
use libtock::memop;

libtock_core::stack_size! {0x400}

#[libtock::main]
async fn main() -> TockResult<()> {
    let drivers = libtock::retrieve_drivers()?;

    drivers.console.create_console();

    let start: u32;
    let end: u32;
    let base_brk: *const u8;
    base_brk = memop::get_brk();

    println!("Starting at break {}.", base_brk as usize);

    unsafe{
      asm!("RDCYCLE {}", out(reg) start);
    }

    for i in 1..10000 {
      let new_brk = base_brk as usize + (i * 8) % 512;
      let _valid = unsafe {memop::set_brk(new_brk as *const u8)};
    }

    unsafe{
      asm!("RDCYCLE {}", out(reg) end);
    }

    println!("Hello Demo Took {} cycles", end - start);

    Ok(())
}
