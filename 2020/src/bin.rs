#[macro_use]
extern crate lazy_static;

mod file_utils;
mod print_utils;
mod day1;
mod day2;
mod day3;
mod day4;
mod day5;
mod day6;

use std::fs::File;
use clap::{Arg, App, ArgMatches};

type SolutionFn = fn(File);
fn come_back_later(_: File) {
    print!("Oops, you are too far in the future.");
}

fn main() {
    let mut solutions:[SolutionFn; 25] = [come_back_later; 25];
    solutions[0] = day1::main;
    solutions[1] = day2::main;
    solutions[2] = day3::main;
    solutions[3] = day4::main;
    solutions[4] = day5::main;
    solutions[5] = day6::main;

    let matches = build_arguments_matcher();
    let day = matches.value_of("day");
    let input = matches.value_of("input");

    let day = day.unwrap().parse::<usize>().unwrap();

    let f = file_utils::open_file(input.unwrap());

    solutions[day - 1](f);
}

fn build_arguments_matcher<'a>() -> ArgMatches<'a> {
    return App::new("Advent Of Code 2020")
        .version("99.99")
        .args(&[
            Arg::with_name("day")
                .short("d")
                .long("day")
                .value_name("DAY")
                .takes_value(true)
                .required(true)
                .possible_values(&[
                    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                    "21", "22", "23", "24"
                ]),
            Arg::with_name("input")
                .short("i")
                .long("input")
                .value_name("INPUT")
                .takes_value(true)
                .required(true)
        ])
        .get_matches();
}
