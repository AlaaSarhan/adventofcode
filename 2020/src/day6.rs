use std::fs::File;
use std::collections::HashMap;
use regex::Regex;
use crate::file_utils::read_grouped_lines;
use crate::print_utils::{print_day, print_task};

lazy_static! {
    static ref EMPTY_LINE: Regex = Regex::new(r"^\s*$").unwrap();
    // Hash map from char to bit fag.
    // 'a' => 1, 'b' => 2, 'c' => 4, 'd' => 8 ... etc
    static ref CHAR_TO_FLAG: HashMap<char, i32> = (b'a'..=b'z').map(|c| {
        (c as char, (2 as i32).pow((c - b'a').into()))
    }).collect();
}

pub fn main(file: File) {
    print_day(6);

    let grouped_lines = read_grouped_lines::<String>(&file, &EMPTY_LINE);

    print_task(1, task1(&grouped_lines));

    print_task(2, task2(&grouped_lines));
}

fn task1(grouped_lines: &Vec<Vec<String>>) -> String {
    grouped_lines.iter().fold(0, |sum, group| {
        sum + group.iter().fold(0, |bitmap, line| {
            line.chars().fold(bitmap, |bitmap, char| {
                bitmap | CHAR_TO_FLAG.get(&char).unwrap_or(&0)
            })
        }).count_ones() as i32
    }).to_string()
}

fn task2(grouped_lines: &Vec<Vec<String>>) -> String {
    grouped_lines.iter().fold(0, |sum, group| {
        sum + group.iter().fold(i32::max_value(), |group_bitmap, line| {
            group_bitmap & line.chars().fold(0, |bitmap, char| {
                bitmap | CHAR_TO_FLAG.get(&char).unwrap_or(&0)
            })
        }).count_ones() as i32
    }).to_string()
}