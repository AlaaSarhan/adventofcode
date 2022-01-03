use std::collections::HashMap;
use std::fs::File;
use crate::print_utils::{print_day, print_task};
use crate::file_utils::read_lines;

lazy_static! {
    static ref CHAR_TO_BIN: HashMap<char, &'static str> = [
        ('B', "1"), ('F', "0"), ('R', "1"), ('L', "0")
    ].iter().cloned().collect();
}

pub fn main(file: File) {
    print_day(5);

    let boarding_passes: Vec<String> = read_lines(&file);
    let mut sorted_seat_ids = boarding_passes_to_seat_ids(boarding_passes);
    sorted_seat_ids.sort();

    print_task(1, task1(&sorted_seat_ids));
    print_task(2, task2(&sorted_seat_ids));
}

fn task1(sorted_seat_ids: &Vec<i32>) -> String {
    sorted_seat_ids.last().unwrap().to_string()
}

fn task2(sorted_seat_ids: &Vec<i32>) -> String {
    let seat_ids_range = *sorted_seat_ids.first().unwrap() ..= *sorted_seat_ids.last().unwrap();
    seat_ids_range.filter(|id| !sorted_seat_ids.contains(id)).nth(0).unwrap().to_string()
}

fn boarding_passes_to_seat_ids(boarding_passes: Vec<String>) -> Vec<i32> {
    boarding_passes.iter().map(|boarding_pass|
        boarding_pass.chars().map(|char| *CHAR_TO_BIN.get(&char).unwrap()).collect::<Vec<&str>>().join("")
    )
        .map(|binary_boarding_pass| i32::from_str_radix(&binary_boarding_pass, 2).unwrap())
        .collect()
}