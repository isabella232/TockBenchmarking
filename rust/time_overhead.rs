// This example just prints "Hello Tock World" to the terminal.
// Run `tockloader listen`, or use any serial program of your choice
//  (e.g. `screen`, `minicom`) to view the message.

#![no_std]
#![feature(asm)]

use libtock::println;
use libtock::result::TockResult;

libtock_core::stack_size! {0x800}

const SAMPLE_COUNT: usize = 32;
const NUMBER_OF_SAMPLES: usize = 32;

fn do_cycle(perf: &libtock::perf::Perf) -> usize {
    
    let start: usize;
    let end: usize;
    let _not_used: usize;
    start = perf.cycles().ok().unwrap();

    for _ in 0..SAMPLE_COUNT {
        perf.cycles().ok();
    }

    end = perf.cycles().ok().unwrap();
    return end - start;
}

fn do_instruction(perf: &libtock::perf::Perf) -> usize {
    
    let start: usize;
    let end: usize;
    let _not_used: usize;
    start = perf.instructions().ok().unwrap();

    for _ in 0..SAMPLE_COUNT {
        perf.instructions().ok();
    }

    end = perf.instructions().ok().unwrap();
    return end - start;
}


#[libtock::main]
async fn main() -> TockResult<()> {
    let drivers = libtock::retrieve_drivers()?;

    drivers.console.create_console();
    let perf = drivers.perf;
    let mut cycle_deltas = [0; NUMBER_OF_SAMPLES];
    let mut instruction_deltas = [0; NUMBER_OF_SAMPLES];

    // do warmup
    for i in 0..NUMBER_OF_SAMPLES {
        cycle_deltas[i] = do_cycle(&perf);
        instruction_deltas[i] = do_instruction(&perf);
    }

    for i in 0..NUMBER_OF_SAMPLES {
        cycle_deltas[i] = do_cycle(&perf);
        instruction_deltas[i] = do_instruction(&perf);
    }

    println!("### RESULTS ###");
    println!("benchmark:, timing overhead cycles");
    println!("description:, Measures cycles taken for samples_per_run+1 calls of the perf.cycles syscall");
    println!("samples_per_run, {}", SAMPLE_COUNT);
    println!("run, cycles");
    for i in 0..NUMBER_OF_SAMPLES {
        println!("{}, {}", i, cycle_deltas[i]);
    }

    println!();

    println!("### RESULTS ###");
    println!("benchmark:, timing overhead instructions");
    println!("description:, Measures instructions taken for samples_per_run+1 calls of the perf.instructions syscall");
    println!("samples_per_run, {}", SAMPLE_COUNT);
    println!("run, instructions");
    for i in 0..NUMBER_OF_SAMPLES {
        println!("{}, {}", i, instruction_deltas[i]);
    }

    Ok(())
}
