--
-- PostgreSQL database dump
--

-- Dumped from database version 10.18
-- Dumped by pg_dump version 10.18

-- Started on 2022-10-18 10:08:59

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 9 (class 2615 OID 59221)
-- Name: administration; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA administration;


ALTER SCHEMA administration OWNER TO postgres;

--
-- TOC entry 8 (class 2615 OID 59239)
-- Name: internal; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA internal;


ALTER SCHEMA internal OWNER TO postgres;

--
-- TOC entry 1 (class 3079 OID 12924)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2839 (class 0 OID 0)
-- Dependencies: 1
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 200 (class 1259 OID 59222)
-- Name: admins; Type: TABLE; Schema: administration; Owner: postgres
--

CREATE TABLE administration.admins (
    id integer NOT NULL,
    login text NOT NULL,
    password text
);


ALTER TABLE administration.admins OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 59240)
-- Name: cart_elements; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.cart_elements (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    add_time timestamp without time zone NOT NULL
);


ALTER TABLE internal.cart_elements OWNER TO postgres;

--
-- TOC entry 198 (class 1259 OID 59184)
-- Name: developers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.developers (
    id integer NOT NULL,
    email text NOT NULL,
    password text NOT NULL,
    name text NOT NULL,
    description text
);


ALTER TABLE public.developers OWNER TO postgres;

--
-- TOC entry 199 (class 1259 OID 59192)
-- Name: games; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.games (
    id integer NOT NULL,
    developer integer NOT NULL,
    name text NOT NULL,
    description text,
    publish_time timestamp without time zone DEFAULT now() NOT NULL,
    price integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.games OWNER TO postgres;

--
-- TOC entry 201 (class 1259 OID 59230)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    login text NOT NULL,
    password text NOT NULL,
    name text,
    register_time timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 2829 (class 0 OID 59222)
-- Dependencies: 200
-- Data for Name: admins; Type: TABLE DATA; Schema: administration; Owner: postgres
--

COPY administration.admins (id, login, password) FROM stdin;
\.


--
-- TOC entry 2831 (class 0 OID 59240)
-- Dependencies: 202
-- Data for Name: cart_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.cart_elements (id, game, "user", add_time) FROM stdin;
\.


--
-- TOC entry 2827 (class 0 OID 59184)
-- Dependencies: 198
-- Data for Name: developers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.developers (id, email, password, name, description) FROM stdin;
\.


--
-- TOC entry 2828 (class 0 OID 59192)
-- Dependencies: 199
-- Data for Name: games; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.games (id, developer, name, description, publish_time, price) FROM stdin;
\.


--
-- TOC entry 2830 (class 0 OID 59230)
-- Dependencies: 201
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, login, password, name, register_time) FROM stdin;
\.


--
-- TOC entry 2698 (class 2606 OID 59229)
-- Name: admins admins_pkey; Type: CONSTRAINT; Schema: administration; Owner: postgres
--

ALTER TABLE ONLY administration.admins
    ADD CONSTRAINT admins_pkey PRIMARY KEY (id);


--
-- TOC entry 2702 (class 2606 OID 59244)
-- Name: cart_elements cart_elements_pkey; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT cart_elements_pkey PRIMARY KEY (id);


--
-- TOC entry 2694 (class 2606 OID 59191)
-- Name: developers developers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers
    ADD CONSTRAINT developers_pkey PRIMARY KEY (id);


--
-- TOC entry 2696 (class 2606 OID 59196)
-- Name: games games_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT games_pkey PRIMARY KEY (id);


--
-- TOC entry 2700 (class 2606 OID 59238)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 2705 (class 2606 OID 59250)
-- Name: cart_elements added by; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT "added by" FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2704 (class 2606 OID 59245)
-- Name: cart_elements refers to; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT "refers to" FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2703 (class 2606 OID 59197)
-- Name: games developed by; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT "developed by" FOREIGN KEY (developer) REFERENCES public.developers(id) ON UPDATE CASCADE ON DELETE CASCADE;


-- Completed on 2022-10-18 10:09:00

--
-- PostgreSQL database dump complete
--

