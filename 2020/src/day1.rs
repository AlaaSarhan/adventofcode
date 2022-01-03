use std::fs::File;
use crate::print_utils::{print_day, print_task};
use crate::file_utils;
use permutate::Permutator;

pub fn main(input: File) {
    let expenses = file_utils::read_lines::<i32>(&input);
    print_day(1);
    print_task(
        1,
        task1(expenses.clone())
    );

    print_task(
        2,
        task2(expenses.clone())
    );
}


fn task1(expenses:Vec::<i32>) -> String {
    let summands = find_summands(expenses, 2, 2020);
    if summands.is_none() { return String::new(); };
    return summands.unwrap().iter().fold(1, |acc, s| acc * s).to_string();
}

fn task2(expenses:Vec::<i32>) -> String {
    let summands = find_summands(expenses, 3, 2020);
    if summands.is_none() { return String::new(); };
    return summands.unwrap().iter().fold(1, |acc, s| acc * s).to_string();
}

fn find_summands(expenses:Vec::<i32>, count:i32, sum:i32) -> Option<Vec<i32>> {
    let expenses: Vec<&i32> = expenses.iter().map(|i| i).collect();

    let mut permutation_source = Vec::<&[&i32]>::new();
    for _ in 0..count {
        permutation_source.push(&expenses);
    }
    let mut permutator = Permutator::new(&permutation_source);

    let mut permutation:Vec<&i32> = vec![&0; count as usize];
    while permutator.next_with_buffer(permutation.as_mut_slice()) {
        let permutation_sum = permutation.iter().fold(0, |acc, s| acc + **s);
        if sum == permutation_sum {
            return Some(permutation.iter().map(|i| **i).collect());
        }
    }
    None
}
