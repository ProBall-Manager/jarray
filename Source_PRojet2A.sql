-- Create the MATCHES table
CREATE TABLE MATCHES (
    IDMATCH NUMBER PRIMARY KEY,
    DATEMATCH DATE,
    LIEU VARCHAR2(100),
    STATUS VARCHAR2(50)
);

-- Create a sequence for auto-incrementing IDMATCH
CREATE SEQUENCE MATCHES_SEQ
START WITH 1
INCREMENT BY 1
NOCACHE
NOCYCLE;

-- Create a trigger to automatically assign the next sequence value
CREATE OR REPLACE TRIGGER MATCHES_TRG
BEFORE INSERT ON MATCHES
FOR EACH ROW
BEGIN
    SELECT MATCHES_SEQ.NEXTVAL
    INTO :NEW.IDMATCH
    FROM DUAL;
END;
/

-- Insert a sample record to test
INSERT INTO MATCHES (DATEMATCH, LIEU, STATUS)
VALUES (TO_DATE('2025-03-15', 'YYYY-MM-DD'), 'Stadium A', 'Scheduled');

-- Query to verify the data was inserted correctly
SELECT * FROM MATCHES;