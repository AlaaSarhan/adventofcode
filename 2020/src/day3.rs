use std::fs::File;
use crate::file_utils;
use crate::print_utils::{print_day, print_task};

pub fn main(input: File) {
    let rows:Vec<String> = file_utils::read_lines(&input);

    print_day(3);
    print_task(
        1,
        task1(&rows).to_string()
    );

    print_task(
        2,
        task2(&rows).to_string()
    );
}

fn task1(rows:&Vec<String>) -> i32 {
    return count_trees_for_slope(rows, 3.0);
}

fn task2(rows:&Vec<String>) -> i32 {
    return count_trees_for_slope(rows, 1.0) *
        count_trees_for_slope(rows, 3.0) *
        count_trees_for_slope(rows, 5.0) *
        count_trees_for_slope(rows, 7.0) *
        count_trees_for_slope(rows, 0.5);
}

fn count_trees_for_slope(rows:&Vec<String>, slope_factor:f32) -> i32 {
    return rows.iter().enumerate().fold(0, |trees_count, (index, row)| {
        let col_step = index as f32 * slope_factor;
        if col_step.fract() != 0.0 { return trees_count }

        let col = if index == 0 { 0 } else { col_step as usize % row.len() };
        if row.chars().nth(col).unwrap() == '.' { trees_count } else { trees_count + 1 }
    });
}