use std::fs::File;
use std::io::{BufReader, BufRead};
use std::str::FromStr;
use std::fmt::Debug;
use regex::Regex;

pub fn open_file(path:&str) -> File {
    return File::open(path).unwrap_or_else(|e| panic!(e));
}

pub fn read_lines<T: FromStr + Debug + Default>(file: &File) -> Vec<T> {
    let mut vec = Vec::<T>::new();
    let buf = BufReader::new(file);

    for line in buf.lines() {
        let line = line.unwrap();
        let line = line.parse::<T>();
        vec.push(line.unwrap_or_default());
    }

    vec
}

pub fn read_grouped_lines<T: FromStr + Debug + Default>(file: &File, separator_line: &Regex) -> Vec<Vec<T>> {
    let mut vec = Vec::<Vec<T>>::new();
    let buf = BufReader::new(file);

    let mut current_group = Vec::<T>::new();
    for line in buf.lines() {
        let line = line.unwrap();
        if separator_line.is_match(&line[..]) {
            if !current_group.is_empty() {
                vec.push(current_group);
                current_group = Vec::<T>::new();
            }
        } else {
            let line = line.parse::<T>();
            current_group.push(line.unwrap_or_default());
        }
    }

    if !current_group.is_empty() {
        vec.push(current_group);
    }

    vec
}
