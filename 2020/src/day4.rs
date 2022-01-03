use std::fs::File;
use crate::print_utils::{print_day, print_task};
use scanner_rust::ScannerAscii;
use std::io::{Seek, SeekFrom};
use regex::Regex;

pub fn main(file: File) {
    print_day(4);

    print_task(1, task1(&file));
    print_task(2, task2(&file));
}

fn task1(mut file: &File) -> String {
    if file.seek(SeekFrom::Start(0)).is_err() { panic!() };
    return count_valid_credentials::<CredentialsPresence>(file).to_string();
}

fn task2(mut file: &File) -> String {
    if file.seek(SeekFrom::Start(0)).is_err() { panic!() };
    return count_valid_credentials::<CredentialsValidity>(file).to_string();
}

fn count_valid_credentials<T: Default + IsValid + SetField>(file: &File) -> i32 {
    let mut scanner = ScannerAscii::new(file);
    let mut valid_passports_count = 0;

    let mut current_credentials: T = T::default();
    loop {
        match scanner.next_until(":") {
            Ok(field) => {
                if field.is_none() {
                    if current_credentials.is_valid() {
                        valid_passports_count += 1;
                    }
                    break;
                }

                let mut captured_field = &field.unwrap()[..];
                if captured_field.starts_with("\n\n") {
                    if current_credentials.is_valid() {
                        valid_passports_count += 1;
                    }
                    current_credentials = T::default();
                }

                captured_field = captured_field.trim();
                let captured_value: String = scanner.next().unwrap_or(Some(String::from(""))).unwrap().to_owned();
                current_credentials.set_field(captured_field.to_string(), captured_value);
            },
            Err(e) => panic!(e)
        }
    }

    return valid_passports_count;
}

const IYR: &str = "iyr";
const BYR: &str = "byr";
const EYR: &str = "eyr";
const HGT: &str = "hgt";
const HCL: &str = "hcl";
const ECL: &str = "ecl";
const PID: &str = "pid";

lazy_static! {
    pub static ref HCL_REGEX: Regex = Regex::new(r"^#([0-9a-f]){6}$").unwrap();
    pub static ref PID_REGEX: Regex = Regex::new(r"^[0-9]{9}$").unwrap();
}

trait SetField {
    fn set_field(&mut self, field: String, value: String);
}

trait IsValid {
    fn is_valid(&self) -> bool;
}

#[derive(Default)]
struct CredentialsPresence {
    iyr: bool,
    byr: bool,
    eyr: bool,
    hgt: bool,
    hcl: bool,
    ecl: bool,
    pid: bool
}

impl IsValid for CredentialsPresence {
    fn is_valid(&self) -> bool {
        return self.eyr && self.pid && self.ecl && self.hcl && self.hgt && self.byr && self.iyr;
    }
}

impl SetField for CredentialsPresence {
    fn set_field(&mut self, field: String, value: String) {
        match &field[..] {
            IYR => self.iyr = !value.is_empty(),
            BYR => self.byr = !value.is_empty(),
            EYR => self.eyr = !value.is_empty(),
            HGT => self.hgt = !value.is_empty(),
            HCL => self.hcl = !value.is_empty(),
            ECL => self.ecl = !value.is_empty(),
            PID => self.pid = !value.is_empty(),
            _ => {}
        }
    }
}

#[derive(Default)]
struct CredentialsValidity {
    iyr: i32,
    byr: i32,
    eyr: i32,
    hgt: String,
    hcl: String,
    ecl: String,
    pid: String
}

impl IsValid for CredentialsValidity {
    fn is_valid(&self) -> bool {
        return (1920..=2020).contains(&self.byr) &&
            (2010..=2020).contains(&self.iyr) &&
            (2020..=2030).contains(&self.eyr) &&
            {
                let hgt = self.hgt.trim();
                if hgt.ends_with("cm") {
                    (150..=193).contains(&self.hgt[0..&self.hgt.len()-2].parse::<i32>().unwrap_or(0))
                } else if hgt.ends_with("in") {
                    (59..=76).contains(&self.hgt[0..&self.hgt.len()-2].parse::<i32>().unwrap_or(0))
                } else {
                    false
                }
            } &&
            HCL_REGEX.is_match(self.hcl.trim()) &&
            ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"].contains(&self.ecl.trim()) &&
            PID_REGEX.is_match(self.pid.trim())
        ;
    }
}

impl SetField for CredentialsValidity {
    fn set_field(&mut self, field: String, value: String) {
        match &field[..] {
            IYR => self.iyr = value.parse::<i32>().unwrap_or(0),
            BYR => self.byr = value.parse::<i32>().unwrap_or(0),
            EYR => self.eyr = value.parse::<i32>().unwrap_or(0),
            HGT => self.hgt = value,
            HCL => self.hcl = value,
            ECL => self.ecl = value,
            PID => self.pid = value,
            _ => {}
        }
    }
}