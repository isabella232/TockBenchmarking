#![no_std]
#![feature(asm)]

use libtock::println;
use libtock::result::TockResult;
use libtock::memop;

libtock_core::stack_size! {0x1800}

const NUMBER_OF_SAMPLES: usize = 256;

#[libtock::main]
async fn main() -> TockResult<()> {
    let drivers = libtock::retrieve_drivers()?;

    drivers.console.create_console();

    let perf = drivers.perf;


    let base_brk: *const u8;
    base_brk = memop::get_brk();

    let mut times = [0 as usize; NUMBER_OF_SAMPLES];
    let mut breaks = [0 as usize; NUMBER_OF_SAMPLES];

    println!("Starting at break {:x}.", base_brk as usize);

    // warmup
    for i in 0..NUMBER_OF_SAMPLES {
      let start: usize;
      let end: usize;
      let new_brk = base_brk as usize + (i * 8) % 4096*8;
      start = perf.cycles().ok().unwrap();
      let _valid = unsafe {memop::set_brk(new_brk as *const u8)};
      end = perf.cycles().ok().unwrap();
      times[i] = end - start;
      breaks[i] = new_brk;
    }
    
    for i in 0..NUMBER_OF_SAMPLES {
      let start: usize;
      let end: usize;
      let new_brk = base_brk as usize + (i * 8) % 4096*8;
      start = perf.cycles().ok().unwrap();
      let _valid = unsafe {memop::set_brk(new_brk as *const u8)};
      end = perf.cycles().ok().unwrap();
      times[i] = end - start;
      breaks[i] = new_brk;
    }

    println!();
    println!("### RESULTS ###");
    println!("benchmark:, setbrk overhead");
    println!("description:, Measures time time to change the break for the stack, this involves updating the MPU mapings");
    println!("base_brk, {:x}", base_brk as usize);
    println!("run, cycles, new_brk, new_brk_base_10");
    for i in 0..NUMBER_OF_SAMPLES {
        println!("{}, {}, {:x}, {}", i, times[i], breaks[i], breaks[i]);
    }


    Ok(())
}
