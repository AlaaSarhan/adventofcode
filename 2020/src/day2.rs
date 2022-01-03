use std::fs::File;
use crate::print_utils;
use crate::file_utils;
use regex::Regex;
use crate::print_utils::print_day;

struct Password {
    value: String,
    required_character: char,
    required_character_qualifier_x: usize,
    required_character_qualifier_y: usize
}

pub fn main(input: File) {
    let passwords:Vec<String> = file_utils::read_lines(&input);
    let passwords = parse_passwords(passwords);

    print_day(2);
    print_utils::print_task(
        1,
        task1(&passwords).to_string()
    );

    print_utils::print_task(
        1,
        task2(&passwords).to_string()
    );
}

fn parse_passwords(passwords:Vec<String>) -> Vec<Password> {
    let regex:Regex = Regex::new(r"^([0-9]+)-([0-9]+) ([a-z]): (.*)$").unwrap();
    return passwords.iter()
        .filter(|s| regex.is_match(s))
        .map(|s| regex.captures(s).unwrap())
        .map(|captures| {
            return Password {
                value: captures.get(4).unwrap().as_str().to_string(),
                required_character: captures.get(3).unwrap().as_str().chars().next().unwrap(),
                required_character_qualifier_x: captures.get(1).unwrap().as_str().parse().unwrap(),
                required_character_qualifier_y: captures.get(2).unwrap().as_str().parse().unwrap()
            };
        }).collect::<Vec::<Password>>();
}

fn task1(passwords: &Vec<Password>) -> i32 {
    return passwords.iter().fold(0, |count, password| {
        let char_count = password.value.chars().fold(0, |char_count, char| {
            return if password.required_character == char { char_count + 1 } else { char_count };
        });

        return if char_count < password.required_character_qualifier_x || char_count > password.required_character_qualifier_y {
            return count;
        } else {
            count + 1
        }
    });
}

fn task2(passwords: &Vec<Password>) -> i32 {
    return passwords.iter().fold(0, |count, password| {
        return match password.value.char_indices().try_fold(0, |match_count, (index, character)| {
            let is_match = character == password.required_character &&
                (index + 1 == password.required_character_qualifier_x || index + 1 == password.required_character_qualifier_y);
            return if is_match && match_count < 1 {
                Some(match_count + 1)
            } else if !is_match {
                Some(match_count)
            } else {
                None
            }
        }) {
            Some(1) => count + 1,
            _ => count
        };
    });
}
