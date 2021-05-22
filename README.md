# PasswordCracker

Program that uses pre-computed (read in @ runtime, everything in the 'input' directory) lookup tables to check password hashes. These data structures store a mapping between the hash of a password, and the correct password for that hash. The hash values are indexed so that it is possible to quickly search the data structure for a given hash. If the hash is present in the structure, the password can be grabbed quickly. This only works for "unsalted" hashes.
