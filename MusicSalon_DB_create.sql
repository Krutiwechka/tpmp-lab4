-- Created by Redgate Data Modeler (https://datamodeler.redgate-platform.com)
-- Last modification date: 2026-05-14 13:37:59.698

-- tables
-- Table: arrivals
CREATE TABLE arrivals (
    id INTEGER NOT NULL CONSTRAINT arrivals_pk PRIMARY KEY,
    operation_date TEXT NOT NULL,
    operation_code TEXT NOT NULL,
    cd_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    CONSTRAINT arrivals_compact_discs FOREIGN KEY (cd_id)
    REFERENCES compact_discs (id)
);

-- Table: compact_discs
CREATE TABLE compact_discs (
    id integer NOT NULL CONSTRAINT compact_discs_pk PRIMARY KEY,
    code text NOT NULL,
    manufacture_date text NOT NULL,
    manufacturer text NOT NULL,
    price real NOT NULL
);

-- Table: musical_pieces
CREATE TABLE musical_pieces (
    id INTEGER NOT NULL CONSTRAINT musical_pieces_pk PRIMARY KEY,
    title TEXT NOT NULL,
    author TEXT NOT NULL,
    performer TEXT NOT NULL,
    cd_id INTEGER NOT NULL,
    CONSTRAINT musical_pieces_compact_discs FOREIGN KEY (cd_id)
    REFERENCES compact_discs (id)
);

-- Table: sales
CREATE TABLE sales (
    id INTEGER NOT NULL CONSTRAINT sales_pk PRIMARY KEY,
    operation_date TEXT NOT NULL,
    operation_code TEXT NOT NULL,
    cd_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    CONSTRAINT sales_compact_discs FOREIGN KEY (cd_id)
    REFERENCES compact_discs (id)
);

-- End of file.

