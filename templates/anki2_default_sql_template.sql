PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE col (
    id              integer primary key,
    crt             integer not null,
    mod             integer not null,
    scm             integer not null,
    ver             integer not null,
    dty             integer not null,
    usn             integer not null,
    ls              integer not null,
    conf            text not null,
    models          text not null,
    decks           text not null,
    dconf           text not null,
    tags            text not null
);
CREATE TABLE notes (
    id              integer primary key,   /* 0 */
    guid            text not null,         /* 1 */
    mid             integer not null,      /* 2 */
    mod             integer not null,      /* 3 */
    usn             integer not null,      /* 4 */
    tags            text not null,         /* 5 */
    flds            text not null,         /* 6 */
    sfld            integer not null,      /* 7 */
    csum            integer not null,      /* 8 */
    flags           integer not null,      /* 9 */
    data            text not null          /* 10 */
);
INSERT INTO notes VALUES(1398130088495,'Ot0!xywPWG',1342697561419,1398130110,-1,'','Bonjour' || CHAR(0x1f) || 'Hello','Bonjour',4077833205,0,'');
INSERT INTO notes VALUES(1398130111274,'OQxYbRc]Dm',1342697561419,1398130117,-1,'','Merci' || CHAR(0x1f) || 'Thank you','Merci',1273459409,0,'');
CREATE TABLE cards (
         id              integer primary key,   /* 0 */
         nid             integer not null,      /* 1 */
         did             integer not null,      /* 2 */
         ord             integer not null,      /* 3 */
         mod             integer not null,      /* 4 */
         usn             integer not null,      /* 5 */
         type            integer not null,      /* 6 */
         queue           integer not null,      /* 7 */
         due             integer not null,      /* 8 */
         ivl             integer not null,      /* 9 */
         factor          integer not null,      /* 10 */
         reps            integer not null,      /* 11 */
         lapses          integer not null,      /* 12 */
         left            integer not null,      /* 13 */
         odue            integer not null,      /* 14 */
         odid            integer not null,      /* 15 */
         flags           integer not null,      /* 16 */
         data            text not null          /* 17 */
);
INSERT INTO cards VALUES(1398130110964,1398130088495,1398130078204,0,1398130110,-1,0,0,484332854,0,0,0,0,0,0,0,0,'');
INSERT INTO cards VALUES(1398130117922,1398130111274,1398130078204,0,1398130117,-1,0,0,353754516,0,0,0,0,0,0,0,0,'');
CREATE TABLE revlog (
    id              integer primary key,
    cid             integer not null,
    usn             integer not null,
    ease            integer not null,
    ivl             integer not null,
    lastIvl         integer not null,
    factor          integer not null,
    time            integer not null,
    type            integer not null
);
CREATE TABLE graves (
    usn             integer not null,
    oid             integer not null,
    type            integer not null
);
ANALYZE sqlite_master;
INSERT INTO "sqlite_stat1" VALUES('col',NULL,'1');
CREATE INDEX ix_notes_usn on notes (usn);
CREATE INDEX ix_cards_usn on cards (usn);
CREATE INDEX ix_revlog_usn on revlog (usn);
CREATE INDEX ix_cards_nid on cards (nid);
CREATE INDEX ix_cards_sched on cards (did, queue, due);
CREATE INDEX ix_revlog_cid on revlog (cid);
CREATE INDEX ix_notes_csum on notes (csum);
COMMIT;
