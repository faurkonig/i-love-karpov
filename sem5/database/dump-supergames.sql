--
-- PostgreSQL database dump
--

-- Dumped from database version 10.21 (Debian 10.21-1.pgdg90+1)
-- Dumped by pg_dump version 14.5

-- Started on 2022-11-06 12:41:40 MSK

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
-- TOC entry 3 (class 2615 OID 16512)
-- Name: administration; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA administration;


ALTER SCHEMA administration OWNER TO postgres;

--
-- TOC entry 4 (class 2615 OID 16513)
-- Name: internal; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA internal;


ALTER SCHEMA internal OWNER TO postgres;

--
-- TOC entry 8 (class 2615 OID 16514)
-- Name: public; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA public;


ALTER SCHEMA public OWNER TO postgres;

--
-- TOC entry 2960 (class 0 OID 0)
-- Dependencies: 8
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'standard public schema';


SET default_tablespace = '';

--
-- TOC entry 199 (class 1259 OID 16636)
-- Name: admins; Type: TABLE; Schema: administration; Owner: postgres
--

CREATE TABLE administration.admins (
    id integer NOT NULL,
    login text NOT NULL,
    password character(32) NOT NULL
);


ALTER TABLE administration.admins OWNER TO postgres;

--
-- TOC entry 198 (class 1259 OID 16634)
-- Name: admins_id_seq; Type: SEQUENCE; Schema: administration; Owner: postgres
--

CREATE SEQUENCE administration.admins_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE administration.admins_id_seq OWNER TO postgres;

--
-- TOC entry 2961 (class 0 OID 0)
-- Dependencies: 198
-- Name: admins_id_seq; Type: SEQUENCE OWNED BY; Schema: administration; Owner: postgres
--

ALTER SEQUENCE administration.admins_id_seq OWNED BY administration.admins.id;


--
-- TOC entry 211 (class 1259 OID 16734)
-- Name: cart_elements; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.cart_elements (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE internal.cart_elements OWNER TO postgres;

--
-- TOC entry 210 (class 1259 OID 16732)
-- Name: cart_elements_id_seq; Type: SEQUENCE; Schema: internal; Owner: postgres
--

CREATE SEQUENCE internal.cart_elements_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE internal.cart_elements_id_seq OWNER TO postgres;

--
-- TOC entry 2962 (class 0 OID 0)
-- Dependencies: 210
-- Name: cart_elements_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.cart_elements_id_seq OWNED BY internal.cart_elements.id;


--
-- TOC entry 213 (class 1259 OID 16753)
-- Name: collection_elements; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.collection_elements (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE internal.collection_elements OWNER TO postgres;

--
-- TOC entry 212 (class 1259 OID 16751)
-- Name: collection_elements_id_seq; Type: SEQUENCE; Schema: internal; Owner: postgres
--

CREATE SEQUENCE internal.collection_elements_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE internal.collection_elements_id_seq OWNER TO postgres;

--
-- TOC entry 2963 (class 0 OID 0)
-- Dependencies: 212
-- Name: collection_elements_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.collection_elements_id_seq OWNED BY internal.collection_elements.id;


--
-- TOC entry 209 (class 1259 OID 16715)
-- Name: friendship; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.friendship (
    id integer NOT NULL,
    source integer NOT NULL,
    target integer NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE internal.friendship OWNER TO postgres;

--
-- TOC entry 208 (class 1259 OID 16713)
-- Name: friendship_id_seq; Type: SEQUENCE; Schema: internal; Owner: postgres
--

CREATE SEQUENCE internal.friendship_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE internal.friendship_id_seq OWNER TO postgres;

--
-- TOC entry 2964 (class 0 OID 0)
-- Dependencies: 208
-- Name: friendship_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.friendship_id_seq OWNED BY internal.friendship.id;


--
-- TOC entry 201 (class 1259 OID 16648)
-- Name: developers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.developers (
    id integer NOT NULL,
    email text NOT NULL,
    login text NOT NULL,
    password character(32) NOT NULL,
    name text NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.developers OWNER TO postgres;

--
-- TOC entry 200 (class 1259 OID 16646)
-- Name: developers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.developers_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.developers_id_seq OWNER TO postgres;

--
-- TOC entry 2965 (class 0 OID 0)
-- Dependencies: 200
-- Name: developers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.developers_id_seq OWNED BY public.developers.id;


--
-- TOC entry 203 (class 1259 OID 16661)
-- Name: games; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.games (
    id integer NOT NULL,
    name text NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    developer integer NOT NULL,
    price double precision DEFAULT 0.0 NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.games OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 16659)
-- Name: games_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.games_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.games_id_seq OWNER TO postgres;

--
-- TOC entry 2966 (class 0 OID 0)
-- Dependencies: 202
-- Name: games_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.games_id_seq OWNED BY public.games.id;


--
-- TOC entry 207 (class 1259 OID 16692)
-- Name: reviews; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.reviews (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    rating smallint NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.reviews OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 16690)
-- Name: reviews_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.reviews_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.reviews_id_seq OWNER TO postgres;

--
-- TOC entry 2967 (class 0 OID 0)
-- Dependencies: 206
-- Name: reviews_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.reviews_id_seq OWNED BY public.reviews.id;


--
-- TOC entry 205 (class 1259 OID 16680)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    login text NOT NULL,
    password character(32) NOT NULL,
    name text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 16678)
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_id_seq OWNER TO postgres;

--
-- TOC entry 2968 (class 0 OID 0)
-- Dependencies: 204
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 2774 (class 2604 OID 16639)
-- Name: admins id; Type: DEFAULT; Schema: administration; Owner: postgres
--

ALTER TABLE ONLY administration.admins ALTER COLUMN id SET DEFAULT nextval('administration.admins_id_seq'::regclass);


--
-- TOC entry 2789 (class 2604 OID 16737)
-- Name: cart_elements id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements ALTER COLUMN id SET DEFAULT nextval('internal.cart_elements_id_seq'::regclass);


--
-- TOC entry 2791 (class 2604 OID 16756)
-- Name: collection_elements id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements ALTER COLUMN id SET DEFAULT nextval('internal.collection_elements_id_seq'::regclass);


--
-- TOC entry 2787 (class 2604 OID 16718)
-- Name: friendship id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship ALTER COLUMN id SET DEFAULT nextval('internal.friendship_id_seq'::regclass);


--
-- TOC entry 2775 (class 2604 OID 16651)
-- Name: developers id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers ALTER COLUMN id SET DEFAULT nextval('public.developers_id_seq'::regclass);


--
-- TOC entry 2778 (class 2604 OID 16664)
-- Name: games id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games ALTER COLUMN id SET DEFAULT nextval('public.games_id_seq'::regclass);


--
-- TOC entry 2784 (class 2604 OID 16695)
-- Name: reviews id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews ALTER COLUMN id SET DEFAULT nextval('public.reviews_id_seq'::regclass);


--
-- TOC entry 2782 (class 2604 OID 16683)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 2940 (class 0 OID 16636)
-- Dependencies: 199
-- Data for Name: admins; Type: TABLE DATA; Schema: administration; Owner: postgres
--

COPY administration.admins (id, login, password) FROM stdin;
\.


--
-- TOC entry 2952 (class 0 OID 16734)
-- Dependencies: 211
-- Data for Name: cart_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.cart_elements (id, game, "user", date) FROM stdin;
\.


--
-- TOC entry 2954 (class 0 OID 16753)
-- Dependencies: 213
-- Data for Name: collection_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.collection_elements (id, game, "user", date) FROM stdin;
\.


--
-- TOC entry 2950 (class 0 OID 16715)
-- Dependencies: 209
-- Data for Name: friendship; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.friendship (id, source, target, date) FROM stdin;
\.


--
-- TOC entry 2942 (class 0 OID 16648)
-- Dependencies: 201
-- Data for Name: developers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.developers (id, email, login, password, name, description, date) FROM stdin;
\.


--
-- TOC entry 2944 (class 0 OID 16661)
-- Dependencies: 203
-- Data for Name: games; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.games (id, name, description, developer, price, date) FROM stdin;
\.


--
-- TOC entry 2948 (class 0 OID 16692)
-- Dependencies: 207
-- Data for Name: reviews; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.reviews (id, game, "user", rating, description, date) FROM stdin;
\.


--
-- TOC entry 2946 (class 0 OID 16680)
-- Dependencies: 205
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, login, password, name, date) FROM stdin;
\.


--
-- TOC entry 2969 (class 0 OID 0)
-- Dependencies: 198
-- Name: admins_id_seq; Type: SEQUENCE SET; Schema: administration; Owner: postgres
--

SELECT pg_catalog.setval('administration.admins_id_seq', 1, false);


--
-- TOC entry 2970 (class 0 OID 0)
-- Dependencies: 210
-- Name: cart_elements_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.cart_elements_id_seq', 1, false);


--
-- TOC entry 2971 (class 0 OID 0)
-- Dependencies: 212
-- Name: collection_elements_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.collection_elements_id_seq', 1, false);


--
-- TOC entry 2972 (class 0 OID 0)
-- Dependencies: 208
-- Name: friendship_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.friendship_id_seq', 1, false);


--
-- TOC entry 2973 (class 0 OID 0)
-- Dependencies: 200
-- Name: developers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.developers_id_seq', 1, false);


--
-- TOC entry 2974 (class 0 OID 0)
-- Dependencies: 202
-- Name: games_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.games_id_seq', 1, false);


--
-- TOC entry 2975 (class 0 OID 0)
-- Dependencies: 206
-- Name: reviews_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.reviews_id_seq', 1, false);


--
-- TOC entry 2976 (class 0 OID 0)
-- Dependencies: 204
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.users_id_seq', 1, false);


--
-- TOC entry 2794 (class 2606 OID 16644)
-- Name: admins admins_pk; Type: CONSTRAINT; Schema: administration; Owner: postgres
--

ALTER TABLE ONLY administration.admins
    ADD CONSTRAINT admins_pk PRIMARY KEY (id);


--
-- TOC entry 2806 (class 2606 OID 16740)
-- Name: cart_elements cart_elements_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT cart_elements_pk PRIMARY KEY (id);


--
-- TOC entry 2808 (class 2606 OID 16759)
-- Name: collection_elements collection_elements_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT collection_elements_pk PRIMARY KEY (id);


--
-- TOC entry 2804 (class 2606 OID 16721)
-- Name: friendship friendship_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT friendship_pk PRIMARY KEY (id);


--
-- TOC entry 2796 (class 2606 OID 16658)
-- Name: developers developers_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers
    ADD CONSTRAINT developers_pk PRIMARY KEY (id);


--
-- TOC entry 2798 (class 2606 OID 16672)
-- Name: games games_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT games_pk PRIMARY KEY (id);


--
-- TOC entry 2802 (class 2606 OID 16702)
-- Name: reviews reviews_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT reviews_pk PRIMARY KEY (id);


--
-- TOC entry 2800 (class 2606 OID 16689)
-- Name: users users_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pk PRIMARY KEY (id);


--
-- TOC entry 2815 (class 2606 OID 16746)
-- Name: cart_elements created_by_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2817 (class 2606 OID 16765)
-- Name: collection_elements created_by_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2814 (class 2606 OID 16741)
-- Name: cart_elements created_on_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2816 (class 2606 OID 16760)
-- Name: collection_elements created_on_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2812 (class 2606 OID 16722)
-- Name: friendship source_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT source_fk FOREIGN KEY (source) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2813 (class 2606 OID 16727)
-- Name: friendship target_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT target_fk FOREIGN KEY (target) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2809 (class 2606 OID 16673)
-- Name: games created_by_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT created_by_fk FOREIGN KEY (developer) REFERENCES public.developers(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2811 (class 2606 OID 16708)
-- Name: reviews created_by_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2810 (class 2606 OID 16703)
-- Name: reviews created_on_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


-- Completed on 2022-11-06 12:41:40 MSK

--
-- PostgreSQL database dump complete
--

